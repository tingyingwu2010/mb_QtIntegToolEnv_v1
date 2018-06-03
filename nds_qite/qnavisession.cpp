#include "qnavisession.h"
#include "navisess_thread.h"
#include "navapi_session.hpp"
#include <QDebug>
#include <QMessageBox>
#include <tuple>

#ifdef WIN32
BOOL IsModuleLoaded(PCWSTR pszModuleName)
{
	// Get the module in the process according to the module name. 
	HMODULE hMod = GetModuleHandle(pszModuleName);
	return (hMod != NULL);
}
#else
#error "the DLL require the WIN32 platform"
#endif

namespace nsNaviSess
{
	enum SessState_E{
		SS_NONE,
		SS_REQ_CALC_ROUTE,
		SS_ACQ_ROUTE_RESULT,
		SS_REQ_EXTRACT_ROUTE_RESULT,
		SS_ACQ_EXTRACT_ROUTE_RESULT,
		SS_DONE
	};

	class CContext : public QSharedData
	{
	public:
		CContext(size_t sid) : m_sid(sid) {}
		CNdsNaviSession m_sess;
		const size_t m_sid;
	};

	static QExplicitlySharedDataPointer<nsNaviSess::CContext> spCurCtx;

	inline QExplicitlySharedDataPointer<CContext> MakeContext(size_t sid)
	{
		return QExplicitlySharedDataPointer<CContext>(new CContext(sid));
	}

	class CCalcResultAcq : public CNaviSessAcquireBase
	{
	public:
		CCalcResultAcq(QExplicitlySharedDataPointer<nsNaviSess::CContext> spCtx)
			: m_spContext(spCtx) {}
		virtual void notify() final
		{
			auto ins = QNaviSession::instance();

			ins->onAcquireRouteCalcResult(m_spContext->m_sid);
		}
		QExplicitlySharedDataPointer<nsNaviSess::CContext> m_spContext;
	};

	class CCalcRouteReq : public CNaviSessRequestBase
	{
	public:
		CCalcRouteReq(size_t sid = 0) : CNaviSessRequestBase(sid) {}
		virtual QExplicitlySharedDataPointer<CNaviSessAcquireBase> getSharedAquirement() final
		{
			return calcRoute();
		}
	private:
		QExplicitlySharedDataPointer<CNaviSessAcquireBase> calcRoute()
		{
			nsNaviSess::spCurCtx = nsNaviSess::MakeContext(sessid);
			CNdsNaviSession& sess = nsNaviSess::spCurCtx->m_sess;

			if (sess.Initialize())
			{
				if (sess.calcRoute(
					1389087203, 476456031, // start pos never change
					//1388266368, 475637856	// only connective level
					1388572928, 476910336	// up level
					)){
					qDebug() << "SID[" << sessid << "] route OK";
				}
				else{
					qDebug() << "SID[" << sessid << "route failure!";
				}
			}

			return MakeQExplicitSharedAcq<CCalcResultAcq>(nsNaviSess::spCurCtx);
		}
	};

	class CExtractedRouteResultAcq : public CNaviSessAcquireBase
	{
	public:
		virtual void notify() final
		{
			auto ins = QNaviSession::instance();
			ins->onAcquireExtractedRouteResult();
		}
	};

	class CExtractRouteReq : public CNaviSessRequestBase
	{
	public:
		CExtractRouteReq(size_t sid = 0) : CNaviSessRequestBase(sid) {}
		virtual QExplicitlySharedDataPointer<CNaviSessAcquireBase> getSharedAquirement() final
		{
			return extractRoute();
		}
	private:
		QExplicitlySharedDataPointer<CNaviSessAcquireBase> extractRoute()
		{

			return MakeQExplicitSharedAcq<CExtractedRouteResultAcq>();
		}

		//QExplicitlySharedDataPointer<CContext> m_spContext;
	};
}

struct QNaviSession::CPrivate
{
	static QMutex mtx;
	CPrivate(QObject* parent)
		: sessid(0) 
		, mSessState(nsNaviSess::SS_NONE)
	{
#ifdef WIN32
		BOOL fLoaded = FALSE;
		// The name of the module to be delay-loaded. 
		PCWSTR pszModuleName = L"NaviApi";

		// Check whether or not the module is loaded. 
		fLoaded = IsModuleLoaded(pszModuleName);
		Q_ASSERT(fLoaded);
#endif

		pThread = new QNaviSessThread(parent);
		pThread->start();
	}

	~CPrivate() {
		if (pThread) delete pThread;
	}

	// retval : isErr, isBusy, isUpdated
	std::tuple<bool, bool, bool> update(nsNaviSess::SessState_E s)
	{
		QMutexLocker locker(&CPrivate::mtx);
		bool isUpdated = false;
		bool isBusy = false;
		bool isError = false;
		auto state_old = mSessState;

		switch (s)
		{
		case nsNaviSess::SS_REQ_CALC_ROUTE:
			if (nsNaviSess::SS_DONE == mSessState || nsNaviSess::SS_NONE == mSessState)
			{
				mSessState = nsNaviSess::SS_REQ_CALC_ROUTE;
				isUpdated = true;
			}
			else if (nsNaviSess::SS_REQ_CALC_ROUTE == mSessState)
			{
				isBusy = true;
			}
			else
			{
				isError = true;
			}
			break;
		case nsNaviSess::SS_ACQ_ROUTE_RESULT:
			if (nsNaviSess::SS_REQ_CALC_ROUTE == mSessState)
			{
				mSessState = nsNaviSess::SS_ACQ_ROUTE_RESULT;
				isUpdated = true;
			}
			else
			{
				isError = true;
			}
			break;
		case nsNaviSess::SS_REQ_EXTRACT_ROUTE_RESULT:
			if (nsNaviSess::SS_ACQ_ROUTE_RESULT)
			{
				mSessState = nsNaviSess::SS_REQ_EXTRACT_ROUTE_RESULT;
				isUpdated = true;
			}
			else if (nsNaviSess::SS_REQ_EXTRACT_ROUTE_RESULT == mSessState)
			{
				isBusy = true;
			}
			else
			{
				isError = true;
			}
			break;
		case nsNaviSess::SS_ACQ_EXTRACT_ROUTE_RESULT:
			if (nsNaviSess::SS_REQ_EXTRACT_ROUTE_RESULT == mSessState)
			{
				mSessState = nsNaviSess::SS_ACQ_EXTRACT_ROUTE_RESULT;
				isUpdated = true;
			}
			else
			{
				isError = true;
			}
			break;
		case nsNaviSess::SS_DONE:
			switch (mSessState)
			{
			case nsNaviSess::SS_REQ_CALC_ROUTE:
			case nsNaviSess::SS_REQ_EXTRACT_ROUTE_RESULT:
				isBusy = true;
				break;
			case nsNaviSess::SS_ACQ_ROUTE_RESULT:
			case nsNaviSess::SS_ACQ_EXTRACT_ROUTE_RESULT:
				mSessState = nsNaviSess::SS_DONE;
				isUpdated = true;
				break;
			case nsNaviSess::SS_NONE:
			case nsNaviSess::SS_DONE:
			default:
				qDebug() << "do nothing";
				break;
			}
			break;
		default:
			break;
		}

		return std::make_tuple(isError, isBusy, isUpdated);
	}

	std::tuple<bool, bool, bool> stepOne()
	{
		QMutexLocker locker(&CPrivate::mtx);
		auto state_old = mSessState;
		locker.unlock();

		bool isErr, isBusy, isUpdated;
		isErr = isBusy = isUpdated = false;
		switch (state_old)
		{
		case nsNaviSess::SS_NONE:
		case nsNaviSess::SS_DONE:
			std::tie(isErr, isBusy, isUpdated) = update(nsNaviSess::SS_REQ_CALC_ROUTE);
			if (isUpdated)
			{
				pThread->sendReq(MakeQExplicitSharedReq<nsNaviSess::CCalcRouteReq>(++sessid));
			}
			break;
		case nsNaviSess::SS_REQ_CALC_ROUTE:
			isBusy = true;
			break;
		case nsNaviSess::SS_ACQ_ROUTE_RESULT:
			std::tie(isErr, isBusy, isUpdated) = update(nsNaviSess::SS_REQ_EXTRACT_ROUTE_RESULT);
			if (isUpdated)
			{
				pThread->sendReq(MakeQExplicitSharedReq<nsNaviSess::CExtractRouteReq>(sessid));
			}
			break;
		case nsNaviSess::SS_ACQ_EXTRACT_ROUTE_RESULT:
			std::tie(isErr, isBusy, isUpdated) = update(nsNaviSess::SS_DONE);
			break;
		default:
			isErr = true;
			qDebug() << __FUNCTIONW__ << "() unexpected session state";
			break;
		}

		return std::make_tuple(isErr, isBusy, isUpdated);
	}

	nsNaviSess::SessState_E mSessState;
	QNaviSessThread* pThread;
	size_t sessid;
};

QMutex QNaviSession::CPrivate::mtx;
QNaviSession::QNaviSession(void) : mp(new CPrivate(this))
{}

QNaviSession::~QNaviSession(void)
{
	delete mp;
}

QNaviSession* QNaviSession::instance()
{
	QMutexLocker locker(&CPrivate::mtx);
	static QNaviSession* pSess = new QNaviSession();
	return pSess;
}

void QNaviSession::onAcquireRouteCalcResult(const size_t sid)
{
	if (nsNaviSess::spCurCtx && sid == nsNaviSess::spCurCtx->m_sid)
	{
		bool isErr, isUpdated, isBusy;
		std::tie(isErr, isBusy, isUpdated) = mp->update(nsNaviSess::SS_ACQ_ROUTE_RESULT);
		qDebug() << "SID[" << sid << "] : get the route result";
	}
	else
	{
		qWarning() << "session id is not consistant!";
	}
}

void QNaviSession::onAcquireExtractedRouteResult()
{
	if (nsNaviSess::spCurCtx)
	{
		bool isErr, isUpdated, isBusy;
		std::tie(isErr, isBusy, isUpdated) = mp->update(nsNaviSess::SS_ACQ_EXTRACT_ROUTE_RESULT);
		if (isUpdated)
		{
			auto spResult = nsNaviSess::spCurCtx->m_sess.getRouteResult();
			const size_t linkNum = spResult->getLinkNum();
			qDebug() << "SID[" << nsNaviSess::spCurCtx->m_sid << "]single section RSlink number is " << linkNum << ".";
			auto spRetResult = std::make_shared<nsNaviSess::CRouteResult>();
			for (size_t i = 0; i < linkNum; i++)
			{
				auto spLink = spResult->getLinkAt(i);
				int x1, y1, x2, y2;
				spLink->getStartPos(x1, y1);
				spLink->getEndPos(x2, y2);

				nsNaviSess::CShapePoint p1(x1, y1), p2(x2, y2);
				nsNaviSess::CResultLink RetLink;
				RetLink.mPosVec.emplace_back(std::move(p1));
				RetLink.mPosVec.emplace_back(std::move(p2));
				spRetResult->mLinkVec.emplace_back(RetLink);
				qDebug() << "#" << i << " " << x1 << "," << y1 << "-" << x2 << "," << y2;
			}
			//emit routeResultUpdated(spRetResult);
			emit routeResultUpdated(spResult);
			qDebug() << "SID[" << nsNaviSess::spCurCtx->m_sid << "] : on acqired the extracted route result";
		}
		else
		{
			qWarning() << "SID[" << nsNaviSess::spCurCtx->m_sid << "] : abnormally on acqired the extracted route result";
		}
	}
	else
	{
		qWarning() << "session id is not consistant!";
	}
}

std::tuple<bool, bool, bool> QNaviSession::test()
{
	return mp->stepOne();
}

std::tuple<bool, bool, bool> QNaviSession::doneTest()
{
	return mp->update(nsNaviSess::SS_DONE);
}
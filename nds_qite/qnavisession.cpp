#include "qnavisession.h"
#include "navisess_thread.h"
#include "NaviApiDll.hpp"
#include <QDebug>
#include <QMessageBox>

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
		SS_REQ_ROUTE,
		SS_DONE
	};

	class CContext : public QSharedData
	{
	public:
		CContext(size_t sid) : m_sid(sid) {}
		CNdsNaviSession m_sess;
		const size_t m_sid;
	};

	inline QExplicitlySharedDataPointer<CContext> MakeContext(size_t sid)
	{
		return QExplicitlySharedDataPointer<CContext>(new CContext(sid));
	}
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

void QNaviSession::onAquireRouteCalcResult(size_t sid)
{
	if (true)
	{
		QMutexLocker locker(&CPrivate::mtx);
		mp->mSessState = nsNaviSess::SS_DONE;
	}
	qDebug() << "SID[" << sid << "] : get the route result";
}

class CNaviSessCalcResultAcq : public CNaviSessAcquireBase
{
public:
	CNaviSessCalcResultAcq(QExplicitlySharedDataPointer<nsNaviSess::CContext> spCtx) 
		: m_spContext(spCtx) {}
	virtual void notify() final
	{
		auto ins = QNaviSession::instance();
		ins->onAquireRouteCalcResult(m_spContext->m_sid);
	}
	QExplicitlySharedDataPointer<nsNaviSess::CContext> m_spContext;
};

class CNaviSessCalcRouteReq : public CNaviSessRequestBase
{
public:
	CNaviSessCalcRouteReq(size_t sid) : CNaviSessRequestBase(sid) {}
	virtual QExplicitlySharedDataPointer<CNaviSessAcquireBase> getSharedAquirement() final
	{
		return calcRoute();
	}
private:
	QExplicitlySharedDataPointer<CNaviSessAcquireBase> calcRoute()
	{
		auto pCtx = nsNaviSess::MakeContext(sessid);
		CNdsNaviSession& sess = pCtx->m_sess;

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

		return MakeQExplicitSharedAcq<CNaviSessCalcResultAcq>(pCtx);
	}
};

void QNaviSession::test()
{
	if (nsNaviSess::SS_DONE == mp->mSessState || nsNaviSess::SS_NONE == mp->mSessState)
	{
		mp->mSessState = nsNaviSess::SS_REQ_ROUTE;
		mp->pThread->sendReq(MakeQExplicitSharedReq<CNaviSessCalcRouteReq>(++mp->sessid));
	}
	else if (nsNaviSess::SS_REQ_ROUTE == mp->mSessState)
	{
		QMessageBox::warning(NULL, "warning", "routing plz wait", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	else
	{
		QMessageBox::warning(NULL, "warning", "unknown state", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}
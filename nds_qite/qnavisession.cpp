#include "qnavisession.h"
#include "navisess_thread.h"
#include "NaviApiDll.hpp"
#include <QDebug>

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


struct QNaviSession::CPrivate
{
	static QMutex mtx;
	CPrivate(QObject* parent)
		: sessid(0) 
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
	QMutexLocker lock(&CPrivate::mtx);
	static QNaviSession* pSess = new QNaviSession();
	return pSess;
}

void QNaviSession::onAquireRouteCalcResult(size_t sid)
{
	qDebug() << "SID[" << sid << "] : get the route result";
}

class CNaviSessCalcResultAcq : public CNaviSessAcquireBase
{
public:
	CNaviSessCalcResultAcq(size_t sid) : CNaviSessAcquireBase(sid) {}
	virtual void notify() final
	{
		auto ins = QNaviSession::instance();
		ins->onAquireRouteCalcResult(sessid);
	}

	void calcRoute()
	{
		CNdsNaviSession sess;

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
	}
};

class CNaviSessCalcRouteReq : public CNaviSessRequestBase
{
public:
	CNaviSessCalcRouteReq(size_t sid) : CNaviSessRequestBase(sid) {}
	virtual QExplicitlySharedDataPointer<CNaviSessAcquireBase> getSharedAquirement() final
	{
		auto pAcq = new CNaviSessCalcResultAcq(sessid);
		pAcq->calcRoute();
		return QExplicitlySharedDataPointer<CNaviSessAcquireBase>(pAcq);
	}
};

void QNaviSession::test()
{
	//mp->pThread->sendReq(QExplicitlySharedDataPointer<CNaviSessRequestBase>(new CNaviSessCalcRouteReq(++mp->sessid)));
	mp->pThread->sendReq(MakeQExplicitSharedReq<CNaviSessCalcRouteReq>(++mp->sessid));
}
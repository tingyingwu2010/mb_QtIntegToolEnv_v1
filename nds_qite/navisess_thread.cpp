#include "navisess_thread.h"

class CNaviSessEndAcq : public CNaviSessAcquireBase
{
public:
	virtual void notify() final {}
};

class CNaviSessEndReq : public CNaviSessRequestBase
{
public:
	virtual QExplicitlySharedDataPointer<CNaviSessAcquireBase> getSharedAquirement() final
	{
		return MakeQExplicitSharedAcq<CNaviSessEndAcq>();
	}
};

QNaviSessThread::QNaviSessThread(QObject *parent) : QThread(parent)
{

}

QNaviSessThread::~QNaviSessThread()
{
	do
	{
		QMutexLocker locker(&mutex);

		queueSharedRequest.clear();
		queueSharedRequest.enqueue(MakeQExplicitSharedReq<CNaviSessEndReq>());
		queueCondition.wakeOne();
	} while (false);

	wait();
}

void QNaviSessThread::run()
{
	QExplicitlySharedDataPointer<CNaviSessRequestBase> pReq;
	forever{
		if(true){
			QMutexLocker locker(&mutex);
			if (queueSharedRequest.isEmpty())
			{
				queueCondition.wait(&mutex);
			}
			pReq = queueSharedRequest.dequeue();
			if (0 == pReq->sessid)
			{
				break;
			}
		}
		pReq->getSharedAquirement()->notify();
	}
}

void QNaviSessThread::sendReq(QExplicitlySharedDataPointer<CNaviSessRequestBase> pReq)
{
	QMutexLocker locker(&mutex);
	queueSharedRequest.enqueue(pReq);
	queueCondition.wakeOne();
}
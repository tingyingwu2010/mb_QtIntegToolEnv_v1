#ifndef NAVISESS_THREAD_H
#define NAVISESS_THREAD_H

#include <QThread>
#include <QQueue>
#include <QWaitCondition>
#include <QExplicitlySharedDataPointer>

class CNaviSessAcquireBase : public QSharedData
{
public:
	CNaviSessAcquireBase(quint64 sid = 0) : sessid(sid) {}
	virtual ~CNaviSessAcquireBase() {}
	virtual void notify() = 0;
	const size_t sessid;
};

class CNaviSessRequestBase : public QSharedData
{
public:
	CNaviSessRequestBase(size_t sid = 0) : sessid(sid) {}
	virtual ~CNaviSessRequestBase() {}
	virtual QExplicitlySharedDataPointer<CNaviSessAcquireBase> getSharedAquirement() = 0;
	const size_t sessid;
};

template <class T, class ...Args> inline
QExplicitlySharedDataPointer<CNaviSessRequestBase> MakeQExplicitSharedReq(Args... Types)
{
	return QExplicitlySharedDataPointer<CNaviSessRequestBase>(new T(Types...));
}

class QNaviSessThread : public QThread
{
	Q_OBJECT

public:
	QNaviSessThread(QObject *parent);
	~QNaviSessThread();
	void sendReq(CNaviSessRequestBase*);
	void sendReq(QExplicitlySharedDataPointer<CNaviSessRequestBase>);
protected:
	void run();
private:
	QQueue< QExplicitlySharedDataPointer<CNaviSessRequestBase> > queueSharedRequest;
	QWaitCondition queueCondition;
	QMutex mutex;
};

#endif // NAVISESS_THREAD_H

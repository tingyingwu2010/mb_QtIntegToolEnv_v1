#ifndef QNAVISESSION_H
#define QNAVISESSION_H

#include <QObject>

class QNaviSession : public QObject
{
	Q_OBJECT

	struct CPrivate;
public:
	QNaviSession(void);
	~QNaviSession(void);
	static QNaviSession* instance();
	void test();
	void onAquireRouteCalcResult(size_t sid);
private:
	CPrivate* mp;
};

#endif // QNAVISESSION_H

#ifndef QNAVISESSION_H
#define QNAVISESSION_H

#include <QObject>
#include <tuple>

class QNaviSession : public QObject
{
	Q_OBJECT

	struct CPrivate;
public:
	QNaviSession(void);
	~QNaviSession(void);
	static QNaviSession* instance();
	std::tuple<bool, bool, bool> test();
	std::tuple<bool, bool, bool> doneTest();
	void onAcquireRouteCalcResult(const size_t sid);
	void onAcquireExtractedRouteResult();
private:
	CPrivate* mp;
};

#endif // QNAVISESSION_H

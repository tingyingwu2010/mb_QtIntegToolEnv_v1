#ifndef QNAVISESSION_H
#define QNAVISESSION_H

#include <QMetaType>
#include <QObject>
#include <memory>
#include <tuple>
#include <vector>

namespace nsNaviSess
{
	struct CShapePoint
	{
		CShapePoint(int _x = 0, int _y = 0) : x(_x), y(_y) {}
		int x, y;
	};

	typedef std::vector<CShapePoint> ShapePoint_vec;

	struct CResultLink
	{
		ShapePoint_vec mPosVec;
	};

	struct CRouteResult
	{
		std::vector<CResultLink> mLinkVec;
	};
};

typedef std::shared_ptr<nsNaviSess::CRouteResult> NaviSessRouteResult_ptr;
Q_DECLARE_METATYPE(NaviSessRouteResult_ptr);

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
signals:
	void routeResultUpdated(NaviSessRouteResult_ptr); // added by wxl
private:
	CPrivate* mp;
};

#endif // QNAVISESSION_H

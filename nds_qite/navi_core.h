#ifndef NAVI_CORE_H
#define NAVI_CORE_H
#if 1 // temporary codes here 
#include <QtWidgets/QtWidgets>
#include <QtCore/QtCore>
#include <QtGui>
#endif
#include <QObject>
#include <cstdint>

#define int16 int16_t
#define int32 int32_t

#if 1 // should be in the other file
struct GDI{};
typedef struct GDI GDI;

struct _Camera2D{};
typedef struct _Camera2D Camera2D;
#endif

#if 1 /// should be in the source codes in cq_basic_type
//////////////////////////////////////////////////////////////////////////
//
typedef struct Point16
{
	int16 x;
	int16 y;
} Point16;

typedef struct Point {
	int32 x, y;
} Point;

typedef struct PointF {
	float x, y;
} PointF;

typedef struct Rect {
	int32 left, top, right, bottom;
} Rect;

typedef struct Size {
	int32 width;
	int32 height;
} Size;

#define SIZE_F_DEFINED

typedef struct SizeF {
	float width;
	float height;
} SizeF;

#define Size_equal(l, r) ((l)->width == (r)->width && (l)->height == (r)->height)

typedef struct Range {
	int32 lower;
	int32 upper;
} Range;
#endif

enum RoadDrawingStyle {
	ERDS_normal,
	ERDS_byPriority,
	ERDS_byUsage,
	ERDS_byLayer,
	ERDS_bySlopeSource,
	ERDS_byTi
};

struct NaviMapDrawOptions
{
	NaviMapDrawOptions()
	{
		drawRoad = true;
		roadDrawingStyle = ERDS_normal;
		layer = 0;
	}
	bool drawRoad; // 是否绘制路网图
	RoadDrawingStyle roadDrawingStyle; // 路网图的绘制风格
	int32 layer; // 路网图的层
};
class NaviMapState
{
public:
	Point worldCenter;
	int scale;
	int rotation;
	bool showTmc;
	int width;
	int height;
};

class NaviMap
{
public:
	//virtual void resize(int w, int h) = 0;
	virtual void draw(QPainter& p, const NaviMapDrawOptions* options) = 0;

#if 0
	void getWorldCenter(int& x, int& y) const;
	virtual void getWorldCenter(Point& pt) const = 0;
	virtual void setWorldCenter(int x, int y) = 0;
	virtual bool getCandidatePoint(Point& pt) const = 0;
	virtual void setCandidatePoint(int x, int y) = 0;

	virtual void viewToWorld(int &x, int &y) = 0;
	virtual void worldToView(int &x, int &y) = 0;

	virtual void getWorldRect(Rect* rct) const = 0;
	virtual void getViewRect(Rect* rct) const = 0;

	virtual int getElevation() = 0;
	virtual void lift(int degree) = 0;

	void setKeepNorthUp(bool keepNorthUp);
	bool getKeepNorthUp() { return m_keepNorthUp; }

	virtual int getRotation() = 0;
	virtual void rotate(int16 degree) = 0;

	virtual int getScale() = 0;
	virtual void setScale(int scale) = 0;

	virtual int getZoomLevel() = 0;
	virtual void setZoomLevel(int zoom) = 0;
	virtual void zoomIn() = 0;
	virtual void zoomOut() = 0;

	virtual void fitRect(const Rect* rect) = 0;
	void fitRectWithCenter(const Rect* rect, const Point* center);

	virtual void enableTmc(bool enable) = 0;

	virtual void reloadStyleSheet() = 0;
	virtual void rebuildRouteOverlays(RouteBase** routes, size_t routeNum) = 0;

	virtual const NaviMapState* getState() = 0;
	virtual void setState(const NaviMapState* state) = 0;

	virtual float pixel2WorldUnit(float pixel) = 0;
#endif
	NaviMap();
	virtual ~NaviMap();

	//virtual void clearCache() = 0;

protected:
	bool m_keepNorthUp;
};

struct NaviCoreEnvPrivate;
class NaviCoreEnv : public QObject
{
	Q_OBJECT

public:
	static NaviCoreEnv* instance();
	static void destroy();
public:
	NaviCoreEnv(QObject *parent);
	~NaviCoreEnv();

public:
	/// 基本的初始化
	void initBasic();
	/// 基本的清理
	void cleanupBasic();

	/// 与国家/地区数据相关的模块的初始化
	void initRegion();
	/// 与国家/地区数据相关的模块的清理
	void cleanupRegion();

	/// 返回是否已经完全初始化(包括基本的初始化和与国家/地区数据相关的模块的初始化)完毕。
	bool isInitialized();

	/// 窗口大小变化后需要调整
	void resize(int w, int h);

	//Eta* getEta();

	/// 当前地图
	NaviMap* map();
protected:
	
	static NaviCoreEnv* _instance;

	NaviCoreEnvPrivate* m_p;
};

#endif // NAVI_CORE_H

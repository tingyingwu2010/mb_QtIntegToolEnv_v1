#pragma once
#include "navi_core.h"
class Navi2DMap :
	public NaviMap
{
public:
	Navi2DMap();
	virtual ~Navi2DMap();
//	virtual void resize(int w, int h);
	virtual void draw(QPainter& p, const NaviMapDrawOptions* options);
#if 0
	virtual void getWorldCenter(Point& pt) const;
	virtual void setWorldCenter(int x, int y);
	virtual bool getCandidatePoint(Point& pt) const;
	virtual void setCandidatePoint(int x, int y);

	virtual void viewToWorld(int &x, int &y);
	virtual void worldToView(int &x, int &y);

	virtual void getWorldRect(Rect* rct) const;
	virtual void getViewRect(Rect* rct) const;

	virtual int getElevation();
	virtual void lift(int degree);

	virtual int getRotation();
	virtual void rotate(int16 degree);

	virtual int getScale();
	virtual void setScale(int scale);

	virtual int getZoomLevel();
	virtual void setZoomLevel(int zoom);
	virtual void zoomIn();
	virtual void zoomOut();

	virtual void fitRect(const Rect* rect);

	virtual void enableTmc(bool enable);

	virtual void reloadStyleSheet();
	//virtual void rebuildRouteOverlays(RouteBase** routes, size_t routeNum);

	virtual const NaviMapState* getState();
	virtual void setState(const NaviMapState* state);

	virtual void clearCache();

	virtual float pixel2WorldUnit(float pixel);
#endif
private:
	GDI* m_gdi;
//	Handle m_surf;
	int m_width;
	int m_height;
//	Handle m_textFont;
	Camera2D m_camera;
	Point m_candidatePoint;
	bool m_hasCandidatePoint;

	NaviMapState m_mapState;

private:
	void drawOnSurface(const NaviMapDrawOptions* options);
};


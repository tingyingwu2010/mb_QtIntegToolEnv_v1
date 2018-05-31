#include "stdafx_qite.h"
#include "navi_2d_map.h"


static void _GDI_drawRoads(GDI* gdi, const Camera2D* camera, RoadDrawingStyle style, int32 layer);


Navi2DMap::Navi2DMap()
{
}


Navi2DMap::~Navi2DMap()
{
}

void Navi2DMap::draw(QPainter& p, const NaviMapDrawOptions* options)
{
	p.setPen(QPen(Qt::black, 12, Qt::DashDotLine, Qt::RoundCap));
	p.setBrush(QBrush(Qt::green, Qt::SolidPattern));
	p.drawLine(80, 80, 400, 420);
	p.drawEllipse(80, 80, 400, 420);
	//resize(m_width, m_height);
	drawOnSurface(options);
}

void Navi2DMap::setGraphicScene(QGraphicsScene* pScene)
{
	m_gdi->mScene = pScene;
}

void Navi2DMap::drawOnSurface(const NaviMapDrawOptions* options)
{
	if (options->drawRoad)
	{
		//GDI_setFont(m_gdi, m_textFont);
		_GDI_drawRoads(m_gdi, &m_camera, options->roadDrawingStyle, options->layer);
	}
}

void _GDI_drawRoads(GDI* gdi, const Camera2D* camera, RoadDrawingStyle style, int32 layer)
{
}


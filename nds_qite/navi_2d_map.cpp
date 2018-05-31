#include "stdafx_qite.h"
#include "navi_2d_map.h"


static void _GDI_drawRoads(GDI* gdi, const Camera2D* camera, RoadDrawingStyle style, int32 layer);

void nsNaviMapGraphics::QRouteLink::paint(QPainter *painter,
	const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen(QPen(Qt::black));
	painter->setBrush(QBrush(Qt::red));

	if (!m_line.isNull())
	{
		painter->drawLine(m_line);
	}
}


static void _GDI_drawRoutes(GDI* gdi)
{
	if (gdi && gdi->mScene->items().empty())
	{
		gdi->mScene->addItem(new nsNaviMapGraphics::QRouteLink(10, 10, 100, 100));
	}
	
}

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

	if (true)
	{
		_GDI_drawRoutes(m_gdi);
	}
}

void _GDI_drawRoads(GDI* gdi, const Camera2D* camera, RoadDrawingStyle style, int32 layer)
{
}


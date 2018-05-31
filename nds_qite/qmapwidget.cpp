#include "stdafx_qite.h"
#include "qmapwidget.h"

QMapWidget::QMapWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	scene = new QGraphicsScene;
	view = new QGraphicsView;
	view->setScene(scene);
	view->setViewport(this);

	NaviMap* map = NaviCoreEnv::instance()->map();
	
}

QMapWidget::~QMapWidget()
{
	
}

void QMapWidget::paintEvent(QPaintEvent *event)
{
	NaviMap* map = NaviCoreEnv::instance()->map();
	if (map == NULL)
		return;
	QPainter painter(this);

	map->draw(painter, &m_options);

	event->accept();
}

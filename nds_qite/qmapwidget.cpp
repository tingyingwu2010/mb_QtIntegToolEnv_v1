#include "stdafx_qite.h"
#include "qmapwidget.h"
#include "qnavisession.h"

namespace nsNaviMapGraphics
{
	class QRouteLink : public QGraphicsLineItem
	{
	public:
		QRouteLink(int x1, int y1, int x2, int y2) : m_line(x1, y1, x2, y2) {}
		void paint(QPainter *painter,
			const QStyleOptionGraphicsItem *option, QWidget *widget);
	private:
		QLine m_line;
	};
}

void nsNaviMapGraphics::QRouteLink::paint(QPainter *painter,
	const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen pen(Qt::black);
	if (option->state & QStyle::State_Selected) {
		pen.setStyle(Qt::DotLine);
		pen.setWidth(2);
	}
	painter->setPen(pen);
	painter->setBrush(QBrush(Qt::red));

	if (!m_line.isNull())
	{
		painter->drawLine(m_line);
	}
}

QMapWidget::QMapWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	scene = new QGraphicsScene;
	view = new QGraphicsView;
	view->setScene(scene);
	view->setViewport(this);

	qRegisterMetaType<NaviSessRouteResult_ptr>("NaviSessRouteResult_ptr");
	//scene->addItem(new nsNaviMapGraphics::QRouteLink(10, 10, 100, 100));
	connect(QNaviSession::instance(), SIGNAL(routeResultUpdated(NaviSessRouteResult_ptr)),
		this, SLOT(onUpdateRoute(NaviSessRouteResult_ptr)));

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

void QMapWidget::onUpdateRoute(NaviSessRouteResult_ptr spResult)
{
	qDebug() << "map widget invoke onUpdateRoute() link number is" << spResult->mLinkVec.size() << ".";

	while (!scene->items().empty())
	{
		delete dynamic_cast<nsNaviMapGraphics::QRouteLink*>(scene->items().last());
		scene->items().removeLast();
	}

	foreach(auto &rLnk, spResult->mLinkVec)
	{
		if (rLnk.mPosVec.size() > 1);
		{
			auto x1 = rLnk.mPosVec[0].x - spResult->mLinkVec[0].mPosVec[0].x;
			auto y1 = rLnk.mPosVec[0].y - spResult->mLinkVec[0].mPosVec[0].y;
			auto x2 = rLnk.mPosVec[1].x - spResult->mLinkVec[0].mPosVec[0].x;
			auto y2 = rLnk.mPosVec[1].y - spResult->mLinkVec[0].mPosVec[0].y;
			auto pRouteLink = new nsNaviMapGraphics::QRouteLink(x1, y1,	x2,	y2);
			scene->addItem(pRouteLink);
		}
	}
}

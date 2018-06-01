#include "stdafx_qite.h"
#include "qmapwidget.h"
#include "qnavisession.h"

namespace nsNaviMapGraphics
{
	class QRouteLink : public QGraphicsLineItem
	{
	public:
		QRouteLink(int x1, int y1, int x2, int y2) : QGraphicsLineItem(x1, y1, x2, y2) {
			setFlags(ItemIsSelectable);
		}
		void paint(QPainter *painter,
			const QStyleOptionGraphicsItem *option, QWidget *widget);
		void ​mousePressEvent(QGraphicsSceneMouseEvent * event);
	};
}

void nsNaviMapGraphics::QRouteLink::​mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	event->accept();
}

void nsNaviMapGraphics::QRouteLink::paint(QPainter *painter,
	const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen newPen(pen());
	qreal penWidth = 4.0 / option->levelOfDetailFromTransform(painter->worldTransform());
	newPen.setWidth(penWidth);
	if (option->state & QStyle::State_Selected) {
		newPen.setStyle(Qt::DotLine);
		newPen.setWidth(penWidth * 2);
	}
	setPen(newPen);
	//painter->drawLine(line());
	return QGraphicsLineItem::paint(painter, option, widget);
}

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH  viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

QMapWidget::QMapWidget(QWidget *parent)
: QGraphicsView(parent)
, m_scale(1.0)
, m_zoomDelta(0.1)
, m_translateSpeed(1.0)
, m_bMouseTranslate(false)
{
	ui.setupUi(this);
	setScene(new QGraphicsScene(this));

	qRegisterMetaType<NaviSessRouteResult_ptr>("NaviSessRouteResult_ptr");

	connect(QNaviSession::instance(), SIGNAL(routeResultUpdated(NaviSessRouteResult_ptr)),
		this, SLOT(onUpdateRoute(NaviSessRouteResult_ptr)));

	NaviMap* map = NaviCoreEnv::instance()->map();
	
	// 去掉滚动条
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setCursor(Qt::PointingHandCursor);
	setRenderHint(QPainter::Antialiasing);

	setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
	centerOn(0, 0);
}

QMapWidget::~QMapWidget()
{
	
}
// 放大
void QMapWidget::zoomIn()
{
	zoom(1 + m_zoomDelta);
}

// 缩小
void QMapWidget::zoomOut()
{
	zoom(1 - m_zoomDelta);
}
// 缩放 - scaleFactor：缩放的比例因子
void QMapWidget::zoom(float scaleFactor)
{
	// 防止过小或过大
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.0007 || factor > 100)
		return;

	scale(scaleFactor, scaleFactor);
	m_scale *= scaleFactor;
}

void QMapWidget::wheelEvent(QWheelEvent *event)
{
	// 滚轮的滚动量
	QPoint scrollAmount = event->angleDelta();
	// 正值表示滚轮远离使用者（放大），负值表示朝向使用者（缩小）
	scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}
#if 1

void QMapWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
	qDebug() << "dbl_clk - x : " << e->x() << ", y : " << e->y();

	QGraphicsView::mouseDoubleClickEvent(e);
}
void QMapWidget::mousePressEvent(QMouseEvent *event)
{
	qDebug() << "dwn - x : " << event->x() << ", y : " << event->y();
	// 当光标底下没有 item 时，才能移动
	QPointF point = mapToScene(event->pos()); 
	m_bMouseTranslate = true;
	m_lastMousePos = event->pos();

	QGraphicsView::mousePressEvent(event);
}
#endif
void QMapWidget::mouseMoveEvent(QMouseEvent *event)
{
	qDebug() << "mov - x : " << event->x() << ", y : " << event->y();
	if (m_bMouseTranslate){
		QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(m_lastMousePos);
		translate(mouseDelta);
	}

	m_lastMousePos = event->pos();

	QGraphicsView::mouseMoveEvent(event);
}

void QMapWidget::mouseReleaseEvent(QMouseEvent *event)
{
	m_bMouseTranslate = false;

	QGraphicsView::mouseReleaseEvent(event);
}

// 平移
void QMapWidget::translate(QPointF delta)
{
	// 根据当前 zoom 缩放平移数
	delta *= m_scale;
	delta *= m_translateSpeed;

	// view 根据鼠标下的点作为锚点来定位 scene
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	QPoint newCenter(VIEW_WIDTH / 2 - delta.x(), VIEW_HEIGHT / 2 - delta.y());
	centerOn(mapToScene(newCenter));

	// scene 在 view 的中心点作为锚点
	setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

void QMapWidget::onUpdateRoute(NaviSessRouteResult_ptr spResult)
{
	qDebug() << "map widget invoke onUpdateRoute() link number is" << spResult->mLinkVec.size() << ".";

	while (!scene()->items().empty())
	{
		delete dynamic_cast<nsNaviMapGraphics::QRouteLink*>(scene()->items().last());
		scene()->items().removeLast();
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
			scene()->addItem(pRouteLink);
		}
	}

	if (!scene()->items().empty())
	{
		//scene()->setSceneRect(scene()->itemsBoundingRect());
	}
}

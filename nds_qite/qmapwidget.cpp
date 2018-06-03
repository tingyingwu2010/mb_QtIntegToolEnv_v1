#include "stdafx_qite.h"
#include "qnavimap_graphics.h"
#include "qmapwidget.h"
#include "qnavisession.h"
#include "qnaviscene.h"


void nsNaviMapGraphics::QRouteLink::​mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	qDebug() << "sdfasdfas";
	if (scene())
	{
		foreach(auto pItm, scene()->selectedItems())
			pItm->setSelected(false);
		setSelected(true);
		event->accept();
	}
}

nsNaviMapGraphics::QRouteLink::QRouteLink(int x0, int y0, std::shared_ptr<CSectResultLinkProxy> pProxy) :  m_pLinkProxy(pProxy)
{
	setFlags(ItemIsSelectable);
	int x1, y1, x2, y2;
	pProxy->getStartPos(x1, y1);
	pProxy->getEndPos(x2, y2);
	setLine(x1 - x0, y1 - y0, x2 - x0, y2 - y0);
}

void nsNaviMapGraphics::QRouteLink::paint(QPainter *painter,
	const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen newPen(pen());
	auto detailLevel = option->levelOfDetailFromTransform(painter->worldTransform());
	qreal penWidth = 4.0 / detailLevel;
	newPen.setWidth(penWidth);
	bool isSelected = option->state & QStyle::State_Selected;
	QStyleOptionGraphicsItem sel_op;
	if (isSelected) {
		newPen.setStyle(Qt::DotLine);
		sel_op.initFrom(widget);
		sel_op.state = QStyle::State_None;;
	}

	setPen(newPen);

	return QGraphicsLineItem::paint(painter, isSelected ? &sel_op : option, widget);
	//return QGraphicsLineItem::paint(painter, option, widget);
}
#if 0
QVariant nsNaviMapGraphics::QRouteLink::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange && scene()) {
	}
	
	return QGraphicsItem::itemChange(change, value);
}
#endif

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
	//setScene(new QGraphicsScene(this));
	setScene(new QNaviScene(this));
	qRegisterMetaType<NaviSessRouteResult_ptr>("NaviSessRouteResult_ptr");
	qRegisterMetaType<NaviSessRouteResult_ptr>("NdsSessRouteResult_ptr");

	//connect(QNaviSession::instance(), SIGNAL(routeResultUpdated(NaviSessRouteResult_ptr)),
	//	this, SLOT(onUpdateRoute(NaviSessRouteResult_ptr)));
	connect(QNaviSession::instance(), SIGNAL(routeResultUpdated(NdsSessRouteResult_ptr)),
		this, SLOT(onUpdateRoute(NdsSessRouteResult_ptr)));

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
	//if ( factor < 0.0007 || factor > 100)
	//	return;

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
void QMapWidget::onUpdateRoute(NdsSessRouteResult_ptr spResult)
{
	qDebug() << "map widget invoke onUpdateRoute() link number is" << spResult->getLinkNum() << ".";

	if (scene())
	{
		auto ps = scene();
		while (!ps->items().empty())
		{
			delete dynamic_cast<nsNaviMapGraphics::QRouteLink*>(ps->items().last());
			ps->items().removeLast();
		}

		int x0, y0, x1, y1, x2, y2;
		QRectF rect0;
		QGraphicsItem* pCenterItm = NULL;
		for (size_t i = 0; i < spResult->getLinkNum(); i++)
		{
			auto pLink = spResult->getLinkAt(i);
			pLink->getStartPos(x1, y1);
			pLink->getEndPos(x2, y2);
			if (0 ==i)
			{
				x0 = x1;
				y0 = y1;
			}
			pCenterItm = new nsNaviMapGraphics::QRouteLink(x0, y0, pLink);
			ps->addItem(pCenterItm);

			QRectF rect2(std::min(x1, x2), std::min(y1, y2), std::abs(x1 - x2), std::abs(y1 - y2));
			if (0 == i)
			{
				rect0 = rect2;
			}
			else
			{
				rect0.united(rect2);
			}
		}

		if (false && rect0.isValid())
		{
			//ps->setSceneRect(rect0);
			auto longer = std::max(rect0.width(), rect0.height());
			m_scale = 100 / longer;
			//setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
			//centerOn(pCenterItm);
			//(QGraphicsView::AnchorViewCenter);
			zoom(m_scale);
		}
	}
}
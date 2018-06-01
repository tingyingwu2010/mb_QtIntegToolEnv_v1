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
	scene = new QGraphicsScene(this);
	setScene(scene);

	qRegisterMetaType<NaviSessRouteResult_ptr>("NaviSessRouteResult_ptr");
	//scene->addItem(new nsNaviMapGraphics::QRouteLink(10, 10, 100, 100));
	connect(QNaviSession::instance(), SIGNAL(routeResultUpdated(NaviSessRouteResult_ptr)),
		this, SLOT(onUpdateRoute(NaviSessRouteResult_ptr)));

	NaviMap* map = NaviCoreEnv::instance()->map();
	
	// ȥ��������
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
// �Ŵ�
void QMapWidget::zoomIn()
{
	zoom(1 + m_zoomDelta);
}

// ��С
void QMapWidget::zoomOut()
{
	zoom(1 - m_zoomDelta);
}
// ���� - scaleFactor�����ŵı�������
void QMapWidget::zoom(float scaleFactor)
{
	// ��ֹ��С�����
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
		return;

	scale(scaleFactor, scaleFactor);
	m_scale *= scaleFactor;
}

void QMapWidget::wheelEvent(QWheelEvent *event)
{
	// ���ֵĹ�����
	QPoint scrollAmount = event->angleDelta();
	// ��ֵ��ʾ����Զ��ʹ���ߣ��Ŵ󣩣���ֵ��ʾ����ʹ���ߣ���С��
	scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}

void QMapWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
	qDebug() << "dbl_clk - x : " << e->x() << ", y : " << e->y();
}

void QMapWidget::mousePressEvent(QMouseEvent *event)
{
	qDebug() << "dwn - x : " << event->x() << ", y : " << event->y();
	// ��������û�� item ʱ�������ƶ�
	QPointF point = mapToScene(event->pos());
	m_bMouseTranslate = true;
	m_lastMousePos = event->pos();
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

// ƽ��
void QMapWidget::translate(QPointF delta)
{
	// ���ݵ�ǰ zoom ����ƽ����
	delta *= m_scale;
	delta *= m_translateSpeed;

	// view ��������µĵ���Ϊê������λ scene
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	QPoint newCenter(VIEW_WIDTH / 2 - delta.x(), VIEW_HEIGHT / 2 - delta.y());
	centerOn(mapToScene(newCenter));

	// scene �� view �����ĵ���Ϊê��
	setTransformationAnchor(QGraphicsView::AnchorViewCenter);
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

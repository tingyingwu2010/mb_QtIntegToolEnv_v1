#ifndef QMAPWIDGET_H
#define QMAPWIDGET_H

#include "navi_core.h"
#include "ui_qmapwidget.h"
#include "qnavisession.h"
//#include "qrouteview.h"

class QMapWidget : public QGraphicsView
{
	Q_OBJECT

public:
	QMapWidget(QWidget *parent = 0);
	~QMapWidget();

protected:
	/////////////////////event method override///////////////////////////////////////
	//virtual void timerEvent(QTimerEvent * event);
//	virtual void paintEvent(QPaintEvent *event);
	void wheelEvent(QWheelEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private:
	Ui::QMapWidget ui;

private:
	NaviMapDrawOptions m_options;	// 地图绘制选项
	qreal m_translateSpeed;  // 平移速度
	qreal m_zoomDelta;  // 缩放的增量
	bool m_bMouseTranslate;  // 平移标识
	QPoint m_lastMousePos;  // 鼠标最后按下的位置
	qreal m_scale;  // 缩放值

	//GraphicsItemChange m_lastChangeItem
public slots:
	void onUpdateRoute(NaviSessRouteResult_ptr);
	void zoomIn();  // 放大
	void zoomOut();  // 缩小
	void zoom(float scaleFactor); // 缩放 - scaleFactor：缩放的比例因子
	void translate(QPointF delta);  // 平移
};

#endif // QMAPWIDGET_H

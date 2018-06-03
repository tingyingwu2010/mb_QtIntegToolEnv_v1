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
	NaviMapDrawOptions m_options;	// ��ͼ����ѡ��
	qreal m_translateSpeed;  // ƽ���ٶ�
	qreal m_zoomDelta;  // ���ŵ�����
	bool m_bMouseTranslate;  // ƽ�Ʊ�ʶ
	QPoint m_lastMousePos;  // �������µ�λ��
	qreal m_scale;  // ����ֵ

	//GraphicsItemChange m_lastChangeItem
public slots:
	void onUpdateRoute(NaviSessRouteResult_ptr);
	void onUpdateRoute(NdsSessRouteResult_ptr);
	void zoomIn();  // �Ŵ�
	void zoomOut();  // ��С
	void zoom(float scaleFactor); // ���� - scaleFactor�����ŵı�������
	void translate(QPointF delta);  // ƽ��
};

#endif // QMAPWIDGET_H

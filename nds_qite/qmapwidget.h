#ifndef QMAPWIDGET_H
#define QMAPWIDGET_H

#include "navi_core.h"
#include "ui_qmapwidget.h"

class QMapWidget : public QWidget
{
	Q_OBJECT

public:
	QMapWidget(QWidget *parent = 0);
	~QMapWidget();

protected:
	/////////////////////event method override///////////////////////////////////////
	//virtual void timerEvent(QTimerEvent * event);
	virtual void paintEvent(QPaintEvent *event);
private:
	Ui::QMapWidget ui;

private:
	NaviMapDrawOptions m_options;	// ��ͼ����ѡ��


};

#endif // QMAPWIDGET_H

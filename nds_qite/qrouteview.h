#ifndef QROUTEVIEW_H
#define QROUTEVIEW_H

#include <QGraphicsView>

class QRouteView : public QGraphicsView
{
	Q_OBJECT

public:
	QRouteView(QWidget *parent = 0);
	~QRouteView();
protected:
	void wheelEvent(QWheelEvent *);
private:
	
};

#endif // QROUTEVIEW_H

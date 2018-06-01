#include "qrouteview.h"
#include <qevent.h>
QRouteView::QRouteView(QWidget *parent)
	: QGraphicsView(parent)
{
	setDragMode(ScrollHandDrag);
}

QRouteView::~QRouteView()
{

}

void QRouteView::wheelEvent(QWheelEvent *e)
{
	double numDegrees = -e->delta() / 8.0;
	double numSteps = numDegrees / 15.0;
	double factor = std::pow(1.125, numSteps);
	scale(factor, factor);
}
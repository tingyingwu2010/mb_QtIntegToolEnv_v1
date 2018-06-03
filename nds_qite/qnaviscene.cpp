#include "qnaviscene.h"
//#include <qgraphicssceneevent.h>
#if 1 // temporary codes here 
#include <QtWidgets/QtWidgets>
#include <QtCore/QtCore>
#include <QtGui>
#endif
#include "qiteapp.h"
#include "qnavimap_graphics.h"

QNaviScene::QNaviScene(QObject *parent)
	: QGraphicsScene(parent)
{
}

QNaviScene::~QNaviScene()
{

}

void QNaviScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QTransform transform;

	auto itmList = selectedItems();
	qDebug() << "scene selected items size " << itmList.size();
	foreach(auto itm, items())
	{
		itm->setSelected(false);
		update();
	}

	auto pItm = itemAt(event->buttonDownScenePos(Qt::LeftButton), transform);
	if (pItm == NULL)
	{
		// do stuff if not clicked on an item
	}
	else
	{
		auto pRouteLinkItm = dynamic_cast<nsNaviMapGraphics::QRouteLink*>(pItm);
		if (pRouteLinkItm)
		{
			//pRouteLinkItm->
			g_app->showRouteLinkInfo();
		}
		QGraphicsScene::mousePressEvent(event); // this forwards the event to the item
	}
}
#pragma once
#if 1 // temporary codes here 
#include <QtWidgets/QtWidgets>
#include <QtCore/QtCore>
#include <QtGui>
#endif
#include "navapi_route.hpp"
namespace nsNaviMapGraphics
{
	class QRouteLink : public QGraphicsLineItem
	{
	public:
		QRouteLink(int x1, int y1, int x2, int y2) : QGraphicsLineItem(x1, y1, x2, y2) {
			setFlags(ItemIsSelectable);
		}
		QRouteLink(int, int, std::shared_ptr<CSectResultLinkProxy> pProxy);
		void paint(QPainter *painter,
			const QStyleOptionGraphicsItem *option, QWidget *widget);
		//QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	protected:
		void ​mousePressEvent(QGraphicsSceneMouseEvent * event);
		std::shared_ptr<CSectResultLinkProxy> m_pLinkProxy;
	};
};

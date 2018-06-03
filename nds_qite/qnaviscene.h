#ifndef QROUTEVIEW_H
#define QROUTEVIEW_H

#include <QGraphicsScene>

class QNaviScene : public QGraphicsScene
{
	Q_OBJECT

public:
	QNaviScene(QObject *parent = 0);
	~QNaviScene();
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent  *);
private:
	
};

#endif // QROUTEVIEW_H

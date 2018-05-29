#ifndef QITEAPP_H
#define QITEAPP_H

#include <QtWidgets/QApplication>
#include "qitewindow.h"
#include "qndslinkattrdialog.h"

class QIteApp : public QApplication
{
	Q_OBJECT
public:
	QIteApp(int &argc, char **argv);
    ~QIteApp();

protected:
	QIteWindow* w;            ///< 主窗口
	QNDSLinkAttrDialog* lad;  ///< 路段属性窗口


public slots:
	// 初始化窗口
	void initWindows(void);
	// 获取主窗口的指针
	QIteWindow* getMainWindow(void);
};


// 获取主窗口的指针.
QIteWindow* AppGetMainWindow(void);

extern QIteApp* g_app;

#endif // QITEAPP_H

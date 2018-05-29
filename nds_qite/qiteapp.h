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
	QIteWindow* w;            ///< ������
	QNDSLinkAttrDialog* lad;  ///< ·�����Դ���


public slots:
	// ��ʼ������
	void initWindows(void);
	// ��ȡ�����ڵ�ָ��
	QIteWindow* getMainWindow(void);
};


// ��ȡ�����ڵ�ָ��.
QIteWindow* AppGetMainWindow(void);

extern QIteApp* g_app;

#endif // QITEAPP_H

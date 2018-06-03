#include "qiteapp.h"

QIteApp* g_app = NULL;
/////////////////////////////////////////////////////////////////////////////
QIteWindow* AppGetMainWindow(void)
{
	return g_app->getMainWindow();
}

QIteApp::QIteApp(int &argc, char **argv) : QApplication(argc, argv)
{
	w = NULL;
}

QIteApp::~QIteApp()
{
	delete w;
}

void QIteApp::initWindows()
{
	if (w == NULL) {
		w = new QIteWindow();

		lad = new QNDSLinkAttrDialog(w);
		lad->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		lad->hide();

		w->addDockWidget(Qt::RightDockWidgetArea, lad);

		
	}
}

void QIteApp::showRouteLinkInfo()
{
	lad->show();
}

QIteWindow* QIteApp::getMainWindow(void)
{
	return w;
}

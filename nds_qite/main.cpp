#include "nds_qite.h"
#include "qiteapp.h"

int main(int argc, char *argv[])
{

#if 0
	QApplication a(argc, argv);
	nds_qite w;
	w.show();

	return a.exec();
#else
	int ret = -1;

	g_app = new QIteApp(argc, argv);

	g_app->initWindows();
	g_app->getMainWindow()->show();

	ret = QApplication::exec();
	delete g_app;
	g_app = NULL;

	return ret;
#endif
}

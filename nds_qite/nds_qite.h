#ifndef NDS_QITE_H
#define NDS_QITE_H

#include <QtWidgets/QMainWindow>
#include "ui_nds_qite.h"

class nds_qite : public QMainWindow
{
	Q_OBJECT

public:
	nds_qite(QWidget *parent = 0);
	~nds_qite();

private:
	Ui::nds_qiteClass ui;
};

#endif // NDS_QITE_H

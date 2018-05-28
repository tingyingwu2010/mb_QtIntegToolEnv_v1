#ifndef QNDSLINKATTRDIALOG_H
#define QNDSLINKATTRDIALOG_H

#include <QDockWidget>
#include "ui_qndslinkattrdialog.h"

class QNDSLinkAttrDialog : public QDockWidget
{
	Q_OBJECT

public:
	QNDSLinkAttrDialog(QWidget *parent = 0);
	~QNDSLinkAttrDialog();

private:
	Ui::QNDSLinkAttrDialog ui;
};

#endif // QNDSLINKATTRDIALOG_H

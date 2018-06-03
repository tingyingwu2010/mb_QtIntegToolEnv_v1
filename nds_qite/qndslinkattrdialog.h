#ifndef QNDSLINKATTRDIALOG_H
#define QNDSLINKATTRDIALOG_H

#include <QDockWidget>
#include "ui_qndslinkattrdialog.h"
#include "navapi_route.hpp"

class QNDSLinkAttrDialog : public QDockWidget
{
	Q_OBJECT

public:
	QNDSLinkAttrDialog(QWidget *parent = 0);
	~QNDSLinkAttrDialog();

	void updateLinkInfo(std::shared_ptr<CSectResultLinkProxy>);
private:
	Ui::QNDSLinkAttrDialog ui;
};

#endif // QNDSLINKATTRDIALOG_H

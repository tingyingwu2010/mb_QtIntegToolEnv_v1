#include "qndslinkattrdialog.h"
#include <qdebug.h>

QNDSLinkAttrDialog::QNDSLinkAttrDialog(QWidget *parent)
	: QDockWidget(parent)
{
	ui.setupUi(this);
}

QNDSLinkAttrDialog::~QNDSLinkAttrDialog()
{

}


void QNDSLinkAttrDialog::updateLinkInfo(std::shared_ptr<CSectResultLinkProxy> spLink)
{
	uint16_t linkId; // link id
	uint32_t tileId;	// packed tile id
	int16_t	 regionId;// region id

	std::tie(linkId, tileId, regionId) = spLink->getLinkId();
	qDebug() << "RegId" << regionId << "tileId" << tileId << "id" << linkId;

	ui.treeWidget->clear();

	QTreeWidgetItem* item = NULL;

	item = new QTreeWidgetItem(ui.treeWidget);
	item->setText(0, tr("Region ID"));
	item->setText(1, QString::number(regionId));

	item = new QTreeWidgetItem(ui.treeWidget);
	item->setText(0, tr("Tile ID"));
	item->setText(1, QString::number(tileId));

	item = new QTreeWidgetItem(ui.treeWidget);
	item->setText(0, tr("Link ID"));
	item->setText(1, QString::number(linkId));

}
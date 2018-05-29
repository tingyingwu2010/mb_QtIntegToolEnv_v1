#ifndef TOOL_BOX_CONTROLLER_H
#define TOOL_BOX_CONTROLLER_H

#include "stdafx_qite.h"

class ToolBoxController : public QObject
{
	Q_OBJECT

public:
	ToolBoxController(QObject *parent);
	~ToolBoxController();

	void attach(QToolBox* toolBox);

	int count() const { return m_infos.size(); }

	QString itemName(int index) const;

	QString itemText(int index) const;
	void setItemText(int index, const QString& text);

	bool isItemVisible(int index) const;
	void setItemVisible(int index, bool visible);

	int indexOf(QWidget* widget) const;
	int indexByName(const QString& name) const;

	void setCurrentIndex(int index);
	void setCurrentByName(const QString& name);
	int currentIndex() const;
	QString currentName() const;

	QStringList getHiddenItemTextList() const;
	void applyHiddenItemTextList(const QStringList& hiddenItemTextList);

private:
	struct ItemInfo
	{
		QString title;
		QWidget* widget;
	};
	QList<ItemInfo> m_infos;
	QToolBox* m_toolBox;
};

#endif // TOOL_BOX_CONTROLLER_H

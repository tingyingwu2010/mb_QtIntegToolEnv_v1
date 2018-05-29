#include "tool_box_controller.h"

ToolBoxController::ToolBoxController(QObject *parent)
	: QObject(parent)
{

}

ToolBoxController::~ToolBoxController()
{

}

void ToolBoxController::attach(QToolBox* toolBox)
{
	m_infos.clear();

	m_toolBox = toolBox;

	for (int i = 0; i < m_toolBox->count(); ++i)
	{
		ItemInfo info;
		info.widget = m_toolBox->widget(i);
		info.title = m_toolBox->itemText(i);
		m_infos.append(info);
	}
}

QString ToolBoxController::itemName(int index) const
{
	if (index < 0 || m_infos.size() <= index)
		return QString();

	return m_infos[index].widget->objectName();
}

QString ToolBoxController::itemText(int index) const
{
	if (index < 0 || m_infos.size() <= index)
		return QString();

	return m_infos[index].title;
}

void ToolBoxController::setItemText(int index, const QString& text)
{
	if (index < 0 || m_infos.size() <= index)
		return;

	m_infos[index].title = text;

	int toolBoxIndex = m_toolBox->indexOf(m_infos[index].widget);
	if (toolBoxIndex >= 0)
		m_toolBox->setItemText(toolBoxIndex, text);
}

bool ToolBoxController::isItemVisible(int index) const
{
	if (index < 0 || m_infos.size() <= index)
		return false;

	QWidget* w = m_infos[index].widget;
	return m_toolBox->indexOf(w) >= 0;
}

void ToolBoxController::setItemVisible(int index, bool visible)
{
	if (index < 0 || m_infos.size() <= index)
		return;

	if (isItemVisible(index) == visible)
		return;

	QWidget* w = m_infos[index].widget;

	if (visible)
	{
		// show
		int insertIndex = -1;
		for (int i = index - 1; i >= 0; --i)
		{
			insertIndex = m_toolBox->indexOf(m_infos[i].widget);
			if (insertIndex >= 0)
			{
				++insertIndex;
				break;
			}
		}
		if (insertIndex < 0)
			insertIndex = 0;

		m_toolBox->insertItem(insertIndex, w, m_infos[index].title);
		w->show();
	}
	else
	{
		// hide
		int indexInToolBox = m_toolBox->indexOf(w);
		Q_ASSERT(indexInToolBox >= 0);
		m_toolBox->removeItem(indexInToolBox);
		w->hide();
	}
}

int ToolBoxController::indexOf(QWidget* widget) const
{
	for (int i = 0; i < m_infos.size(); ++i)
	{
		if (m_infos[i].widget == widget)
			return i;
	}
	return -1;
}

int ToolBoxController::indexByName(const QString& name) const
{
	for (int i = 0; i < m_infos.size(); ++i)
	{
		if (m_infos[i].widget->objectName() == name)
			return i;
	}
	return -1;
}

void ToolBoxController::setCurrentIndex(int index)
{
	if (index < 0 || m_infos.size() <= index)
		return;

	QWidget* w = m_infos[index].widget;
	int indexInToolBox = m_toolBox->indexOf(w);

	if (indexInToolBox >= 0)
		m_toolBox->setCurrentIndex(indexInToolBox);
}

void ToolBoxController::setCurrentByName(const QString& name)
{
	setCurrentIndex(indexByName(name));
}

int ToolBoxController::currentIndex() const
{
	return indexOf(m_toolBox->currentWidget());
}

QString ToolBoxController::currentName() const
{
	QWidget* w = m_toolBox->currentWidget();
	if (w)
		return w->objectName();
	else
		return QString();
}

QStringList ToolBoxController::getHiddenItemTextList() const
{
	QStringList l;

	for (int i = 0; i < m_infos.size(); ++i)
	{
		if (!isItemVisible(i))
			l.append(m_infos[i].widget->objectName());
	}
	return l;
}

void ToolBoxController::applyHiddenItemTextList(const QStringList& hiddenItemTextList)
{
	for (int i = 0; i < m_infos.size(); ++i)
	{
		setItemVisible(i, !hiddenItemTextList.contains(m_infos[i].widget->objectName()));
	}
}

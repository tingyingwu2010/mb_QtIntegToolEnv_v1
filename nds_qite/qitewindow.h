#ifndef QITEWINDOW_H
#define QITEWINDOW_H

#include <QMainWindow>
#include "ui_qitewindow.h"
#include "tool_box_controller.h"

class QIteWindow : public QMainWindow
{
	Q_OBJECT

public:
	QIteWindow(QWidget *parent = 0);
	~QIteWindow();

	void postInitialize();

private:
	bool m_isNaviCoreInitialized; // 引擎是否已初始化的标记

private:
	void setupToolBox();

public slots:
	void onRoutePlanDevTestBtnClicked();

protected:
	virtual bool event(QEvent * e);
	// To show/hide ToolBox item dynamically.
public:
	ToolBoxController* getToolBoxController() { return m_toolBoxController; }
private:
	ToolBoxController* m_toolBoxController;

private:
	Ui::QIteWindowClass ui;
};

#endif // QITEWINDOW_H

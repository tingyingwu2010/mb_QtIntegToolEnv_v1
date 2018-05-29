#include "qitewindow.h"
#include "data_center.h"

QIteWindow::QIteWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_isNaviCoreInitialized(false)
{
	ui.setupUi(this);
}

QIteWindow::~QIteWindow()
{

}

void QIteWindow::postInitialize()
{
}

bool QIteWindow::event(QEvent * e)
{
	if (e->type() == QEvent::WinIdChange)
	{
		//g_hWnd = (HWND)effectiveWinId();

		if (!m_isNaviCoreInitialized)
		{
			NaviCoreEnv* e = NaviCoreEnv::instance();

			//e->initBasic();
			e->initRegion();

			m_isNaviCoreInitialized = true;
		}
	}

	return QMainWindow::event(e);
}

void QIteWindow::setupToolBox()
{

	DataCenter* dc = DataCenter::instance();
	const QIteOptions* opts = dc->getOptions();

	m_toolBoxController = new ToolBoxController(this);
	m_toolBoxController->attach(ui.toolBox);
	// Set current page of toolBox.
	int toolBoxCurrentIndex = m_toolBoxController->indexByName(opts->toolBoxCurrentPage);
	if (toolBoxCurrentIndex == -1)
	{
		toolBoxCurrentIndex = opts->toolBoxCurrentIndex;
	}
	if (toolBoxCurrentIndex < m_toolBoxController->count())
		m_toolBoxController->setCurrentIndex(toolBoxCurrentIndex);

	// Apply hidden pages of toolBox
	m_toolBoxController->applyHiddenItemTextList(opts->toolBoxHiddenPages);
}

void QIteWindow::onRoutePlanDevTestBtnClicked()
{
#if 0
	QMessageBox msg;
	msg.setText("HelloWord!");
	msg.exec();
#endif
	bool isErr, isBusy;
	DataCenter* dc = DataCenter::instance();
	std::tie(isErr, isBusy, std::ignore) = dc->test();
	if (isErr)
	{
		QMessageBox::warning(this, "warning", "unexpect behavior!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	else if (isBusy)
	{
		QMessageBox::information(NULL, "Title", "worker is busy!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}

void QIteWindow::onDevTestDoneBtnClicked()
{
	bool isErr, isBusy;
	DataCenter* dc = DataCenter::instance();
	std::tie(isErr, isBusy, std::ignore) = dc->doneTest();
	if (isErr)
	{
		QMessageBox::warning(this, "warning", "unexpect behavior!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	else if (isBusy)
	{
		QMessageBox::information(NULL, "Title", "worker is busy!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}
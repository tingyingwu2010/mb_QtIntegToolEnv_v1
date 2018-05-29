#ifndef DATA_CENTER_H
#define DATA_CENTER_H

#include "stdafx_qite.h"

/**	@brief 路线绘制风格 */
typedef enum RouteDrawingStyle
{
	RouteDrawingStyle_simple = 0, ///< 原ITE的红绿相间简单风格
	RouteDrawingStyle_client = 1  ///< PND及手机导航风格
} RouteDrawingStyle;

/** @brief 道路属性绘制选项*/
enum RoadDrawingOption
{
	RoadDrawingOption_nomal = 0,		///< 仅绘制基本路形
	RoadDrawingOption_drawAngle = 1,	///< 在道路上标记道路角度
};

/**	@brief 路线相关设置 */
struct RouteSettings
{
	RouteSettings() : roadOption(RoadDrawingOption_nomal), style(RouteDrawingStyle_simple) {}
	RoadDrawingOption roadOption;
	RouteDrawingStyle style;
};

struct QIteOptions
{
	RouteSettings routeSettings;

	int toolBoxCurrentIndex;        ///< QITE 工具箱中的当前页下标，已废弃，当 toolBoxCurrentPage 不可用时才使用
	QString toolBoxCurrentPage;     ///< QITE 工具箱中的当前页名称
	QStringList toolBoxHiddenPages; ///< QITE 工具箱中隐藏不显示的页的名称列表
};

class DataCenter : public QObject
{
	Q_OBJECT
public:
	static DataCenter* instance();
	static void cleanup();
	void loadOptions(QSettings& s);
	void saveOptions(QSettings& s);

	/*===================================================================
	Options
	===================================================================*/
signals:
	void optionsChanged(const QIteOptions*);
public:
	const QIteOptions* getOptions(void) { return &m_options; }
	void setOptions(const QIteOptions* options);
	void applyOptions(void);

	std::tuple<bool, bool, bool> test();
	std::tuple<bool, bool, bool> doneTest();

private:
	QIteOptions m_options;

	/*===================================================================
	Constructor/Destructor
	===================================================================*/
protected:
	DataCenter(void);
	~DataCenter(void);
	static DataCenter* _instance;

	friend class QMapWidget;


};

#endif // DATA_CENTER_H

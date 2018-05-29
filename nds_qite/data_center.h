#ifndef DATA_CENTER_H
#define DATA_CENTER_H

#include "stdafx_qite.h"

/**	@brief ·�߻��Ʒ�� */
typedef enum RouteDrawingStyle
{
	RouteDrawingStyle_simple = 0, ///< ԭITE�ĺ������򵥷��
	RouteDrawingStyle_client = 1  ///< PND���ֻ��������
} RouteDrawingStyle;

/** @brief ��·���Ի���ѡ��*/
enum RoadDrawingOption
{
	RoadDrawingOption_nomal = 0,		///< �����ƻ���·��
	RoadDrawingOption_drawAngle = 1,	///< �ڵ�·�ϱ�ǵ�·�Ƕ�
};

/**	@brief ·��������� */
struct RouteSettings
{
	RouteSettings() : roadOption(RoadDrawingOption_nomal), style(RouteDrawingStyle_simple) {}
	RoadDrawingOption roadOption;
	RouteDrawingStyle style;
};

struct QIteOptions
{
	RouteSettings routeSettings;

	int toolBoxCurrentIndex;        ///< QITE �������еĵ�ǰҳ�±꣬�ѷ������� toolBoxCurrentPage ������ʱ��ʹ��
	QString toolBoxCurrentPage;     ///< QITE �������еĵ�ǰҳ����
	QStringList toolBoxHiddenPages; ///< QITE �����������ز���ʾ��ҳ�������б�
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

	void test();

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

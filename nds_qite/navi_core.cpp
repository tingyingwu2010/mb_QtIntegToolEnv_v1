#include "stdafx_qite.h"
#include "navi_core.h"
#include "qitewindow.h"
#include "navi_2d_map.h"

NaviCoreEnv* NaviCoreEnv::_instance = NULL;

struct NaviCoreEnvPrivate
{
	//Handle m_mapFonts[3];
	//Handle m_textFont;
	//Handle m_bigFont;
	//Handle m_digitFont;

	//Point bakCenter;
	//int bakScale;

	NaviCoreEnvPrivate();
	~NaviCoreEnvPrivate();

	Navi2DMap* m_2dMap;
	NaviMap* m_map;

	//Eta* m_eta;

	//bool m_isInitialized;
};

NaviCoreEnvPrivate::NaviCoreEnvPrivate()
{
	m_2dMap = NULL;
	m_map = NULL;
	//m_eta = NULL;
	//m_isInitialized = false;
}

NaviCoreEnvPrivate::~NaviCoreEnvPrivate()
{
	delete m_2dMap;
	m_map = NULL;
}

NaviCoreEnv::NaviCoreEnv(QObject *parent)
	: QObject(parent)
{
	m_p = new NaviCoreEnvPrivate();
}

NaviCoreEnv::~NaviCoreEnv()
{
	delete m_p;
}

NaviCoreEnv* NaviCoreEnv::instance()
{
	if (_instance == NULL)
	{
		_instance = new NaviCoreEnv(NULL);
	}
	return _instance;
}

void NaviCoreEnv::destroy()
{
	delete _instance;
	_instance = NULL;
}

void NaviCoreEnv::initRegion()
{
	if (m_p->m_2dMap == NULL)
	{
		m_p->m_2dMap = new Navi2DMap();
	}

	m_p->m_map = m_p->m_2dMap;
}

NaviMap* NaviCoreEnv::map()
{
	return m_p->m_map;
}

NaviMap::NaviMap()
{
	m_keepNorthUp = true;
}

NaviMap::~NaviMap()
{

}
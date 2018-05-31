#include "data_center.h"
#include "qnavisession.h"

DataCenter* DataCenter::_instance = NULL;

DataCenter* DataCenter::instance()
{
	if (_instance == NULL)
	{
		_instance = new DataCenter();
	}

	return _instance;
}

void DataCenter::cleanup()
{
	if (_instance != NULL)
	{
		delete _instance;
		_instance = NULL;
	}
}

DataCenter::DataCenter(void)
{

}

DataCenter::~DataCenter()
{

}

std::tuple<bool, bool, bool> DataCenter::test()
{
	return QNaviSession::instance()->test();
}

std::tuple<bool, bool, bool> DataCenter::doneTest()
{
	return QNaviSession::instance()->doneTest();
}

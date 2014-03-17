#include "DataSyncThread.h"

#include "api/OplkQtApi.h"

DataSyncThread::DataSyncThread(ProcessImageParser &parser)
{
	this->in = &(static_cast<ProcessImageIn&>(parser.GetProcessImage(Direction::PI_IN)));
	this->out = &(static_cast<ProcessImageOut&>(parser.GetProcessImage(Direction::PI_OUT)));
}

void DataSyncThread::run()
{
	for (;;)
	{
		tOplkError oplkRet = kErrorGeneralError;
		oplkRet = DataSyncThread::ProcessSync();
		if (oplkRet != kErrorOk)
		{
			return;
		}
		QThread::msleep(500);
	}
}

tOplkError DataSyncThread::ProcessSync()
{
	tOplkError oplkRet = kErrorGeneralError;
	oplkRet = oplk_exchangeProcessImageOut();
	if (oplkRet != kErrorOk)
	{
		return oplkRet;
	}

	emit SignalUpdateInputValues();
	emit SignalUpdateOutputValues();

	oplkRet = oplk_exchangeProcessImageIn();
	return oplkRet;
}

#ifndef _DATASYNCTHREAD_H_
#define _DATASYNCTHREAD_H_

#include <QThread>
#include <oplk/errordefs.h>
#include "ProcessImageVariables.h"
#include "ProcessImageMemory.h"

#include <user/processimage/ProcessImageParser.h>

class DataSyncThread : public QThread
{
	Q_OBJECT

public:
	DataSyncThread(ProcessImageParser &parser);
	void run();

signals:
	void SignalUpdateInputValues();
	void SignalUpdateOutputValues();

private:
	ProcessImageIn *in;
	ProcessImageOut *out;

	DataSyncThread(const DataSyncThread& syncThread);
	DataSyncThread& operator=(const DataSyncThread& syncThread);

	tOplkError ProcessSync();
};

#endif // _DATASYNCTHREAD_H_

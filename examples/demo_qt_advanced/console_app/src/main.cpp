#include <QCoreApplication>

#include <Epl.h>

#include "api/OplkQtApi.h"

int main(int argc, char *argv[])
{
	QCoreApplication application(argc, argv);
	qDebug("------------------------------------------------------\n");
	qDebug("  WELCOME TO OPEN POWERLINK VERSION 2.0 console demo  \n");
	qDebug("------------------------------------------------------\n");
	//To find the GUID of the network interface: from cmd prompt type: netsh diag gui [Enter]
	std::string devName = "\\Device\\NPF_{E9C6B2B9-4ACE-46D4-AA39-63E11E37D017}";
	tEplKernel ret;

	ret = OplkQtApi::InitStack(240, devName);
	if(ret != kEplSuccessful)
	{
		qDebug("InitStack retCode %x", ret);
	}

	ret = OplkQtApi::StartStack();
	if(ret != kEplSuccessful)
	{
		qDebug("StartStack retCode %x", ret);
	}

	application.exec();
	return 0;
}

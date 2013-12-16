#include <QCoreApplication>
#define _WINSOCKAPI_
//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <Epl.h>
#include <pcap.h>
#include "api/OplkQtApi.h"

#include "../include/Receiver.h"

int main(int argc, char *argv[])
{
	QCoreApplication application(argc, argv);
	pcap_if_t* alldevs;
	char sErr_Msg[PCAP_ERRBUF_SIZE];
	pcap_findalldevs(&alldevs, sErr_Msg);
	qDebug("------------------------------------------------------\n");
	qDebug("  WELCOME TO OPEN POWERLINK VERSION 2.0 console demo  \n");
	qDebug("------------------------------------------------------\n");
	//To find the GUID of the network interface: from cmd prompt type: netsh diag gui [Enter]
	std::string devName = "\\Device\\NPF_{E9C6B2B9-4ACE-46D4-AA39-63E11E37D017}";
	tEplKernel ret;

	Receiver receiver1;
	int methodIndex1 = receiver1.metaObject()->indexOfMethod(QMetaObject::normalizedSignature("HandleNodeStateChanged(const int, tNmtState)").constData());
	int methodIndex2 = receiver1.metaObject()->indexOfMethod(QMetaObject::normalizedSignature("HandleLocalNodeStateChanged(tNmtState)").constData());
	qDebug("Method index1: %d", methodIndex1);
	qDebug("Method index2: %d", methodIndex2);
	OplkQtApi::RegisterNodeStateChangedEventHandler(receiver1, receiver1.metaObject()->method(methodIndex1));
	OplkQtApi::RegisterLocalNodeStateChangedEventHandler(receiver1, receiver1.metaObject()->method(methodIndex2));

	/* RegisterEventLogger */
	OplkQtApi::RegisterEventLogger(receiver1, receiver1.metaObject()->method(
		receiver1.metaObject()->indexOfMethod(
					QMetaObject::normalizedSignature(
							"HandleLogEvent(const QString&)").constData())));

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

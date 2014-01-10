#include <QCoreApplication>

#if (TARGET_SYSTEM == _WIN32_)
#define _WINSOCKAPI_
#endif  // (TARGET_SYSTEM == _WIN32_)

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <Epl.h>
#include <pcap.h>

#include "api/OplkQtApi.h"

#include "../include/Receiver.h"
#include "../include/ConsoleReader.h"

#ifdef CONFIG_USE_PCAP
tEplKernel selectPcapDevice(char *pDevName_p);
#endif // CONFIG_USE_PCAP

int main(int argc, char *argv[])
{
	QCoreApplication application(argc, argv);
	tEplKernel ret;

	qDebug("------------------------------------------------------\n");
	qDebug("  WELCOME TO OPEN POWERLINK VERSION 2.0 console demo  \n");
	qDebug("------------------------------------------------------\n");
	//To find the GUID of the network interface: from cmd prompt type: netsh diag gui [Enter]
	//std::string devName = "\\Device\\NPF_{E9C6B2B9-4ACE-46D4-AA39-63E11E37D017}";
	std::string devName;
	devName.reserve(128);

#ifdef CONFIG_USE_PCAP
	ret = selectPcapDevice(&devName[0]);
	qDebug("PCAP select retCode %d", devName.size());
#else
	devName = "epl";
#endif

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

	qDebug("------------------------------------------------------\n");
	qDebug("-----------------Choose any options:------------------\n");
	qDebug("\t r to execute SDO read\n");
	qDebug("\t w to execute SDO Write\n");
	qDebug("------------------------------------------------------\n");
	ConsoleReader *consoleReaderObj = new ConsoleReader();
	consoleReaderObj->start();

	application.exec();
	delete consoleReaderObj;
	return 0;
}

//TODO(RaM): To use examples/common/pcap-console.c
tEplKernel selectPcapDevice(char *pDevName_p)
{
	char            sErr_Msg[ PCAP_ERRBUF_SIZE ];
	pcap_if_t *     alldevs;
	pcap_if_t *     seldev;
	int             i = 0;
	int             inum;

	/* Retrieve the device list on the local machine */
	if (pcap_findalldevs(&alldevs, sErr_Msg) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", sErr_Msg);
		return kEplNoResource;
	}

	qDebug("List of Ethernet Cards Found in this System: \n");
	qDebug("--------------------------------------------------\n");
	for (seldev = alldevs; seldev != NULL; seldev = seldev->next)
	{
		++i;
		if (seldev->description)
		{
			qDebug("%d. %s\n %s\n", i, seldev->description, seldev->name);
		}
		else
		{
			qDebug("%d. %s\n", i, seldev->name);
		}
	}

	if (i == 0)
	{
		qDebug("\nNo interfaces found! Make sure pcap library is installed.\n");
		return kEplNoResource;
	}

	qDebug("--------------------------------------------------\n");
	qDebug("Select the interface to be used for POWERLINK (1-%d):",i);
	if (scanf("%d", &inum) == EOF)
	{
		qDebug("Scanf fail");
		pcap_freealldevs(alldevs);
		return kEplNoResource;
	}

	qDebug("--------------------------------------------------\n");
	if ((inum < 1) || (inum > i))
	{
		qDebug("\nInterface number out of range.\n");
		pcap_freealldevs(alldevs);
		return kEplNoResource;
	}

	/* Jump to the selected adapter */
	for (seldev = alldevs, i = 0; i < (inum - 1); seldev = seldev->next, i++)
	{   // do nothing
	}
	strncpy(pDevName_p, seldev->name, 127);
	qDebug("%s", pDevName_p);
	return kEplSuccessful;
}

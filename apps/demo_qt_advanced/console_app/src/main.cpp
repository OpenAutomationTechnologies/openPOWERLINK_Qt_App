#include <QCoreApplication>
#if (TARGET_SYSTEM == _WIN32_)
#define _WINSOCKAPI_
#endif  // (TARGET_SYSTEM == _WIN32_)

#include <pcap.h>
//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <Epl.h>

#include "api/OplkQtApi.h"

#include "../include/Receiver.h"
#include "../include/ConsoleReader.h"

int main(int argc, char *argv[])
{
	QCoreApplication application(argc, argv);
	tEplKernel ret;

	qDebug("------------------------------------------------------\n");
	qDebug("  WELCOME TO OPEN POWERLINK VERSION 2.0 console demo  \n");
	qDebug("------------------------------------------------------\n");
	//To find the GUID of the network interface: from cmd prompt type: netsh diag gui [Enter]
	//std::string devName = "\\Device\\NPF_{E9C6B2B9-4ACE-46D4-AA39-63E11E37D017}";

	Receiver receiver1;
	int methodIndex1 = receiver1.metaObject()->indexOfMethod(QMetaObject::normalizedSignature("HandleNodeStateChanged(const int, tNmtState)").constData());
	int methodIndex2 = receiver1.metaObject()->indexOfMethod(QMetaObject::normalizedSignature("HandleLocalNodeStateChanged(tNmtState)").constData());
	OplkQtApi::RegisterNodeStateChangedEventHandler(receiver1, receiver1.metaObject()->method(methodIndex1));
	OplkQtApi::RegisterLocalNodeStateChangedEventHandler(receiver1, receiver1.metaObject()->method(methodIndex2));

	/* RegisterEventLogger */
	OplkQtApi::RegisterEventLogger(receiver1, receiver1.metaObject()->method(
		receiver1.metaObject()->indexOfMethod(
					QMetaObject::normalizedSignature(
							"HandleLogEvent(const QString&)").constData())));


	qDebug("-----------------Choose any options:------------------\n");
	qDebug("\t S to Start/Stop POWERLINK STACK\n");
	qDebug("\t N to execute NMT command\n");
	qDebug("\t R to execute SDO read\n");
	qDebug("\t W to execute SDO Write\n");
	qDebug("\t T to Display PDO data for all nodes\n");
	qDebug("\t I to Exchange Input ProcessImage\n");
	qDebug("\t O to Exchange Output ProcessImage\n");
	qDebug("------------------------------------------------------\n");
	ConsoleReader *consoleReaderObj = new ConsoleReader();
	consoleReaderObj->start();

	application.exec();
	delete consoleReaderObj;
	return 0;
}

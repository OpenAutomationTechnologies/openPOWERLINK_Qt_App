/**
********************************************************************************
\file main.cpp

\brief Main module of the QT MN demo application
		The file contains the main source code of the QT MN demo application

\author Ramakrishnan Periyakaruppan

\copyright (c) 2014, Kalycito Infotech Private Limited
					 All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holders nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/*******************************************************************************
* INCLUDES
*******************************************************************************/

#include <QCoreApplication>

#include <oplk/oplk.h>
#ifdef __unix__
#include <common/target.h>
#endif

#include "api/OplkQtApi.h"

#include "../include/Receiver.h"
#include "../include/ConsoleReader.h"

int main(int argc, char *argv[])
{
	#ifdef __unix__
		target_init();
	#endif

	QCoreApplication application(argc, argv);

	qDebug("------------------------------------------------------\n");
	qDebug("  WELCOME TO OPEN POWERLINK VERSION 2.0 console demo  \n");
	qDebug("------------------------------------------------------\n");

	Receiver receiver1;
	int methodIndex1 = receiver1.metaObject()->indexOfMethod(
						QMetaObject::normalizedSignature(
							"HandleNodeStateChanged(const int, tNmtState)").constData());
	int methodIndex2 = receiver1.metaObject()->indexOfMethod(
						QMetaObject::normalizedSignature(
							"HandleLocalNodeStateChanged(tNmtState)").constData());
	OplkQtApi::RegisterNodeStateChangedEventHandler(
				receiver1, receiver1.metaObject()->method(methodIndex1));
	OplkQtApi::RegisterLocalNodeStateChangedEventHandler(
				receiver1, receiver1.metaObject()->method(methodIndex2));

	int methodIndex3 = receiver1.metaObject()->indexOfMethod(
						QMetaObject::normalizedSignature(
							"HandleNodeFound(const int)").constData());
	OplkQtApi::RegisterNodeFoundEventHandler(receiver1,
					receiver1.metaObject()->method(methodIndex3));

	/* RegisterEventLogger */
	OplkQtApi::RegisterEventLogger(receiver1, receiver1.metaObject()->method(
		receiver1.metaObject()->indexOfMethod(QMetaObject::normalizedSignature(
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

/**
********************************************************************************
\file   MainWindow.cpp

\brief  The main window of the application.

The file contains the definitions of all the layouts present in the mainwindow
and for the widgets and handles the menu actions and toolbar actions.

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
#include <fstream>

#include <QMessageBox>
#include "QDesktopServices"

#include <oplk/debugstr.h>

#include "api/OplkQtApi.h"
#include "common/XmlParserException.h"

#include "MainWindow.h"
#include "AboutDialog.h"

/*******************************************************************************
* Module global variables
*******************************************************************************/
const UINT localNodeId = 240; /// Local node id is set to MN nodeid. This application is designed to run as MN only.


/*******************************************************************************
* Public functions
*******************************************************************************/

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	sdo(SdoTransfer()),
	log(LoggerWindow()),
	cdcDialog(DialogOpenCdc()),
	networkInterface(SelectNwInterfaceDialog()),
	nmtCmd(NmtCommandsDock()),
	nodeStatus(NodeStatusDock()),
	piVar(ProcessImageVariables()),
	piMemory(ProcessImageMemory()),
	parser(NULL),
	status(StatusBar())
{
	this->ui.setupUi(this);
	this->setStatusBar(&(this->status));
	this->ui.actionStop->setDisabled(true);
	int index = this->status.metaObject()->indexOfMethod(
						QMetaObject::normalizedSignature(
						"UpdateNmtStatus(tNmtState)").constData());
	Q_ASSERT(index != -1);
	// If asserted check for the Function name

	this->sdo.setEnabled(false);
	this->nmtCmd.setEnabled(false);
	this->piVar.setEnabled(false);
	this->piMemory.setEnabled(false);

	this->ui.tabWidget->addTab(&(this->piVar), "ProcessImage Variables view");

	bool ret = connect(this, SIGNAL(SignalStackStopped()),
				&(this->piVar), SLOT(ResetView()));
	Q_ASSERT(ret != false);

	this->ui.tabWidget->addTab(&(this->piMemory), "ProcessImage Memory view");

	ret = connect(this, SIGNAL(SignalStackStopped()),
				&(this->piMemory), SLOT(ResetView()));
	Q_ASSERT(ret != false);

	this->addDockWidget(Qt::RightDockWidgetArea, &(this->nmtCmd));
	this->setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
	this->setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea );
	this->setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
	this->setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );
	this->nmtCmd.show();


	this->addDockWidget(Qt::RightDockWidgetArea, &(this->nodeStatus));
	this->nodeStatus.show();

	this->addDockWidget(Qt::BottomDockWidgetArea, &(this->log));
	this->log.show();

	this->ui.tabWidget->addTab(&(this->sdo), "SDO Transfer");


	ret = OplkQtApi::RegisterLocalNodeStateChangedEventHandler(this->status,
							this->status.metaObject()->method(index));
	Q_ASSERT(ret != false);

	index = this->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(
						"HandleCriticalError(const QString&)").constData());
	Q_ASSERT(index != -1);

	ret = OplkQtApi::RegisterCriticalErrorEventHandler(*(this),
							this->metaObject()->method(index));
	Q_ASSERT(ret != false);

	ret = connect(&(this->nodeStatus),
				  SIGNAL(SignalNodeAvailable(unsigned int)),
				  &(this->sdo),
				  SLOT(UpdateNodeList(unsigned int)));
	Q_ASSERT(ret != false);

	ret = connect(&(this->nodeStatus),
				  SIGNAL(SignalNodeNotActive(unsigned int)),
				  &(this->sdo),
				  SLOT(RemoveFromNodeList(unsigned int)));
	Q_ASSERT(ret != false);

	ret = connect(&(this->cdcDialog),
				  SIGNAL(SignalCdcChanged(QString&)),
				  &(this->status),
				  SLOT(SetCdcFilePath(QString&)));
	Q_ASSERT(ret != false);

	ret = connect(&(this->cdcDialog),
				  SIGNAL(SignalXapChanged(QString&)),
				  &(this->status),
				  SLOT(SetXapFilePath(QString&)));
	Q_ASSERT(ret != false);

	ret = connect(&(this->networkInterface),
				  SIGNAL(SignalNetworkInterfaceChanged(const QString&)),
				  &(this->status),
				  SLOT(SetNetworkInterfaceName(const QString&)));
	Q_ASSERT(ret != false);
}

MainWindow::~MainWindow()
{
	delete this->parser;
}

/*******************************************************************************
* Private functions
*******************************************************************************/

void MainWindow::on_actionOpen_CDC_triggered()
{
	this->cdcDialog.exec();
}

void MainWindow::on_actionQuit_triggered()
{
	// TODO discuss. Oplk Stop?
	this->close();
}

bool MainWindow::on_actionSelect_Interface_triggered()
{
	if (this->networkInterface.FillList() < 0)
	{
		QMessageBox::warning(this, "PCAP not working!",
							 "No PCAP interfaces found!\n"
							 "Make sure LibPcap is installed and you have root permissions!",
							 QMessageBox::Close);
		return false;
	}

	if (this->networkInterface.exec() == QDialog::Rejected)
	{
		return false;
	}

	return true;
}

void MainWindow::on_actionStart_triggered()
{
	// No need to save the return value of the addTab

	if ((this->cdcDialog.GetCdcFileName().isEmpty())
		|| (this->cdcDialog.GetXapFileName().isEmpty()))
	{
		if (this->cdcDialog.exec() == QDialog::Rejected)
		{
			QMessageBox::critical(this, QStringLiteral("CDC, Xap.xml not found"),
								 QString(QStringLiteral("CDC file and xap.xml not found")),
								 QMessageBox::Close);
			return;
		}
	}

	try
	{
		this->parser = ProcessImageParser::NewInstance(ProcessImageParserType::QT_XML_PARSER);

		if (this->cdcDialog.GetXapFileName().isEmpty())
		{
			QMessageBox::critical(this, "File not found",
								 QString("xap.xml not found"),
								 QMessageBox::Close);
			return;
		}
		std::ifstream ifsXap(this->cdcDialog.GetXapFileName().toStdString().c_str());
		std::string xapData((std::istreambuf_iterator<char>(ifsXap)), std::istreambuf_iterator<char>());
		this->parser->Parse(xapData.c_str());
	}
	catch(const std::exception& ex)
	{
		QMessageBox::critical(this, QStringLiteral("Xml Parsing failed!"),
							 QString("XmlReader has found errors with your xap file.\n Error: %1 ")
							  .arg(ex.what()),
							 QMessageBox::Close);
		qDebug("An Exception has occured: %s", ex.what());
		return;
	}

	ProcessImageIn& piIn = static_cast<ProcessImageIn&>(this->parser->GetProcessImage(Direction::PI_IN));
	ProcessImageOut& piOut = static_cast<ProcessImageOut&>(this->parser->GetProcessImage(Direction::PI_OUT));

	//TODO Start powerlink and only if success enable the stop button.
	if (this->networkInterface.GetDevName().isEmpty())
	{
		if (!this->on_actionSelect_Interface_triggered())
		{
			return;
		}
	}

	tOplkError oplkRet = kErrorGeneralError;
	oplkRet = OplkQtApi::InitStack(localNodeId, this->networkInterface.GetDevName().toStdString());
	if (oplkRet != kErrorOk)
	{
		QMessageBox::critical(this, "Init Powerlink failed",
							 QString("Init Powerlink with error: %1 ")
							  .arg(debugstr_getRetValStr(oplkRet)),
							 QMessageBox::Close);
		qDebug("InitStack retCode %x", oplkRet);
		return;
	}

	std::string cdc = this->cdcDialog.GetCdcFileName().toStdString();
	oplkRet = OplkQtApi::SetCdc(cdc);
	if (oplkRet != kErrorOk)
	{
		QMessageBox::critical(this, "SetCDC failed",
							 QString("SetCDC failed: %1 ")
							  .arg(debugstr_getRetValStr(oplkRet)),
							 QMessageBox::Close);
		return;
	}

	oplkRet = OplkQtApi::AllocateProcessImage(piIn, piOut);
	if (oplkRet != kErrorOk)
	{
		QMessageBox::critical(this, "ProcessImage allocation failed",
							 QString("ProcessImage allocation with error: %1 ")
							  .arg(debugstr_getRetValStr(oplkRet)),
							 QMessageBox::Close);
		qDebug("AllocateProcessImage retCode %x", oplkRet);
		return;
	}

	// After successful allocation of processimage prepare the user interface.
	this->piVar.SetProcessImage(&piIn, &piOut);
	this->piMemory.SetProcessImage(&piIn, &piOut);
	
	oplkRet = OplkQtApi::StartStack();
	if (oplkRet != kErrorOk)
	{
		QMessageBox::critical(this, "Start Powerlink failed",
							 QString("Start Powerlink failed with error: %1 ")
							  .arg(debugstr_getRetValStr(oplkRet)),
							 QMessageBox::Close);
		qDebug("StartStack retCode %x", oplkRet);
		return;
	}

	this->ui.actionOpen_CDC->setEnabled(false);
	this->ui.actionSelect_Interface->setEnabled(false);
	this->ui.actionStop->setEnabled(true);
	this->ui.actionStart->setEnabled(false);
	this->piVar.setEnabled(true);
	this->piMemory.setEnabled(true);

	this->sdo.setEnabled(true);
	this->nmtCmd.setEnabled(true);
}

void MainWindow::on_actionStop_triggered()
{
	emit SignalStackStopped();
	tOplkError oplkRet = OplkQtApi::StopStack();
	if (oplkRet != kErrorOk)
	{
		QMessageBox::critical(this, "Stop Powerlink failed",
							 QString("Stop Powerlink failed with error: %1 ")
							  .arg(debugstr_getRetValStr(oplkRet)),
							 QMessageBox::Close);
		qDebug("StopStack retCode %x", oplkRet);

		// Test this usecase for return;
		return;
	}

	this->ui.actionOpen_CDC->setEnabled(true);
	this->ui.actionSelect_Interface->setEnabled(true);
	this->ui.actionStart->setEnabled(true);
	this->ui.actionStop->setEnabled(false);

	this->sdo.setEnabled(false);
	this->nmtCmd.setEnabled(false);
	this->piVar.setEnabled(false);
	this->piMemory.setEnabled(false);

	delete this->parser;
}

void MainWindow::on_actionAbout_triggered()
{
	// No need to handle return
	AboutDialog(this).exec();
}

void MainWindow::on_actionHelp_triggered()
{
	QString helpUrl = "http://openpowerlink.sourceforge.net";
	if (!QDesktopServices::openUrl(QUrl(helpUrl)))
	{
		QMessageBox::warning(this, "Web Browser not found",
						QString("Make sure you have any default web browser.\n If not manually go to the link %1").arg(helpUrl),
							 QMessageBox::Close);
	}
}

void MainWindow::HandleCriticalError(const QString& errorMessage)
{
	this->on_actionStop_triggered();
	QMessageBox::warning(this, "Critical Error!",
						QString("Critical error has occured in the openPOWERLINK stack.\nError: %1")
								.arg(errorMessage),
						QMessageBox::Close);
}

#include "MainWindow.h"
#include <QMessageBox>
#include "AboutDialog.h"
#include "QDesktopServices"

#include "api/OplkQtApi.h"
#include "user/processimage/ProcessImageParser.h"
#include "common/XmlParserException.h"

#include <oplk/debugstr.h>

#include <fstream>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	sdoTab(new SdoTransfer()),
	log(new LoggerWindow()),
	cdcDialog(new DialogOpenCdc()),
	nwInterfaceDialog(new SelectNwInterfaceDialog()),
	nmtCmdWindow(new NmtCommandsDock()),
	cnStatus(new NodeStatusDock()),
	mnNode(new NodeUi("MN - 240")),
	piVar(NULL),
	piMemory(NULL)
{
	this->ui.setupUi(this);

	this->ui.actionStop->setDisabled(true);
	this->ui.actionRestart->setEnabled(false);

	this->ui.toolBar->setStyleSheet("QToolBar { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
							  stop: 0 #6B7FBF, stop: 0.4 #DDDDDD, \
							  stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3); \
							  border-radius: 5px; \
							  spacing: 3px; } ");

}

MainWindow::~MainWindow()
{
	delete this->sdoTab;
	delete this->log;
	delete this->nwInterfaceDialog;
	delete this->cdcDialog;
	delete this->nmtCmdWindow;
	delete this->cnStatus;
	delete this->mnNode;
	if (!this->piVar)
	{
		delete this->piVar;
	}

	if (!this->piMemory)
	{
		delete this->piMemory;
	}
}

void MainWindow::on_actionToggle_Full_Screen_triggered()
{
	if (isFullScreen())
		showNormal();
	else
		showFullScreen();
}

void MainWindow::on_actionOpen_CDC_triggered()
{
	this->cdcDialog->exec();
}

void MainWindow::on_actionQuit_triggered()
{
	this->close();
}

bool MainWindow::on_actionSelect_Interface_triggered()
{
	if (this->nwInterfaceDialog->FillList() < 0)
	{
		QMessageBox::warning(this, "PCAP not working!",
							 "No PCAP interfaces found!\n"
							 "Make sure LibPcap is installed and you have root permissions!",
							 QMessageBox::Close);
		return false;
	}

	if (this->nwInterfaceDialog->exec() == QDialog::Rejected)
	{
		return false;
	}
	// qDebug("Nw Name = %s ", qPrintable(this->nwInterfaceDialog->getDevName()));
	return true;
}

void MainWindow::on_actionStart_triggered()
{
	// No need to save the return value of the addTab

	this->ui.statusbar->addPermanentWidget(this->mnNode);
	this->mnNode->show();

	ProcessImageParser *pi = NULL;

	try
	{
		pi = ProcessImageParser::NewInstance(ProcessImageParserType::QT_XML_PARSER);
		std::ifstream ifsXap("xap.xml");
		std::string xapData((std::istreambuf_iterator<char>(ifsXap)), std::istreambuf_iterator<char>());
		pi->Parse(xapData.c_str());
	}
	catch(const std::exception& ex)
	{
		QMessageBox::critical(this, "Xml Parsing failed!",
							 QString("xap.xml has found errors with your xap.xml.\n Error: %1 ")
							  .arg(ex.what()),
							 QMessageBox::Close);
		qDebug("An Exception has occured: %s", ex.what());
	}

	ProcessImageIn *piIn = &(static_cast<ProcessImageIn&>(pi->GetProcessImage(Direction::PI_IN)));
	ProcessImageOut *piOut = &(static_cast<ProcessImageOut&>(pi->GetProcessImage(Direction::PI_OUT)));

	//TODO Start powerlink and only if success enable the stop button.
	if (this->nwInterfaceDialog->GetDevName() == "")
	{
		if (!this->on_actionSelect_Interface_triggered())
		{
			return;
		}
	}

	tOplkError oplkRet = kErrorGeneralError;
	oplkRet = OplkQtApi::InitStack(240, this->nwInterfaceDialog->GetDevName().toStdString());
	if (oplkRet != kErrorOk)
	{
		QMessageBox::critical(this, "Init Powerlink failed",
							 QString("Init Powerlink with error: %1 ")
							  .arg(debugstr_getRetValStr(oplkRet)),
							 QMessageBox::Close);
		qDebug("InitStack retCode %x", oplkRet);
		return;
	}

	oplkRet = OplkQtApi::AllocateProcessImage(*piIn, *piOut);
	if (oplkRet != kErrorOk)
	{
		QMessageBox::critical(this, "ProcessImage allocation failed",
							 QString("ProcessImage allocation with error: %1 ")
							  .arg(debugstr_getRetValStr(oplkRet)),
							 QMessageBox::Close);
		qDebug("AllocateProcessImage retCode %x", oplkRet);
		return;
	}

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

	this->ui.statusbar->showMessage(this->nwInterfaceDialog->GetDevDescription());

	this->ui.actionStop->setEnabled(true);
	this->ui.actionRestart->setEnabled(true);
	this->ui.actionStart->setEnabled(false);
// 	this->ui.statusbar->showMessage("asdfaskdasfsdfksdf");

	this->addDockWidget(Qt::RightDockWidgetArea, this->nmtCmdWindow);
	this->setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
	this->setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea );
	this->setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
	this->setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );
	this->nmtCmdWindow->show();
	this->addDockWidget(Qt::RightDockWidgetArea, this->cnStatus);
	this->cnStatus->show();

	this->addDockWidget(Qt::BottomDockWidgetArea, this->log);
	this->log->show();

	this->mnNode->SetNodeStatus(0);

	this->ui.tabWidget->addTab(this->sdoTab, "SDO Transfer");

	/* Moved to DataInOutThread and create ui there.*/
	this->piVar = new ProcessImageVariables();
	this->ui.tabWidget->addTab(this->piVar, "ProcessImage Variables view");

	this->piMemory = new ProcessImageMemory();
	this->ui.tabWidget->addTab(this->piMemory, "ProcessImage Memory view");
}

void MainWindow::on_actionStop_triggered()
{
	tOplkError oplkRet = OplkQtApi::StopStack();
	if (oplkRet != kErrorOk)
	{
		QMessageBox::critical(this, "Stop Powerlink failed",
							 QString("Stop Powerlink failed with error: %1 ")
							  .arg(debugstr_getRetValStr(oplkRet)),
							 QMessageBox::Close);
		qDebug("StopStack retCode %x", oplkRet);
		return;
	}

	this->ui.actionStart->setEnabled(true);
	this->ui.actionRestart->setEnabled(false);
	this->ui.actionStop->setEnabled(false);

	// this->sdoTab->hide();
	this->removeDockWidget(this->cnStatus);
	this->removeDockWidget(this->log);
	this->removeDockWidget(this->nmtCmdWindow);
	// MN Status is not hidden.

	this->ui.tabWidget->removeTab(1);
	this->ui.tabWidget->removeTab(2);
	this->ui.tabWidget->removeTab(3);

	delete this->piVar;
	delete this->piMemory;
}

void MainWindow::on_actionRestart_triggered()
{
// NMT restart
	 tOplkError oplkRet = oplk_execNmtCommand(kNmtEventSwReset);
	// tOplkError oplkRet =  OplkQtApi::ExecuteNmtCommand(240, kNmtCmdSwReset);
	if (oplkRet != kErrorOk)
	{
		QMessageBox::critical(this, "Restart Powerlink failed",
							 QString("Restart Powerlink failed with error: %1 ")
							  .arg(debugstr_getRetValStr(oplkRet)),
							 QMessageBox::Close);
	}
}

void MainWindow::on_actionAbout_triggered()
{
	// No need to handle return
	AboutDialog(this).exec();
}

void MainWindow::on_actionHelp_triggered()
{
	QString helpUrl = "http://openpowerlink.sourceforge.net";
	if (!QDesktopServices::openUrl(QUrl(helpUrl, QUrl::TolerantMode)))
	{
		QMessageBox::warning(this, "Web Browser not found",
						QString("Make sure you have any default web browser.\n If not manually go to the link %1").arg(helpUrl),
							 QMessageBox::Close);
	}
}

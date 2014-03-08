#include "MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	this->ui.setupUi(this);
	this->nwInterfaceDialog = NULL;
	this->ui.actionStop->setDisabled(true);
	this->ui.actionRestart->setEnabled(false);

	this->ui.toolBar->setStyleSheet("QToolBar { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
							  stop: 0 #6B7FBF, stop: 0.4 #DDDDDD, \
							  stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3); \
							  border-radius: 5px; \
							  spacing: 3px; } ");

	this->sdoTab = new SdoTransfer();
	this->ui.tabWidget->addTab(this->sdoTab, "SDO Transfer");

	this->piVar = new ProcessImageVariables();
	this->ui.tabWidget->addTab(this->piVar, "ProcessImage Variables view");

	this->piMemory = new ProcessImageMemory();
	this->ui.tabWidget->addTab(this->piMemory, "ProcessImage Memory view");
	//this->addDockWidget(static_cast<Qt::DockWidgetArea>(8), sdoTab);

	this->nmtCmdWindow = new NmtCommandsDock();
	this->addDockWidget(Qt::RightDockWidgetArea, this->nmtCmdWindow);
	this->setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
	this->setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea );
	this->setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
	this->setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

	this->cnStatus = new NodeStatusDock();
	this->addDockWidget(Qt::RightDockWidgetArea, this->cnStatus);

	this->log = new LogerWindow();
	this->addDockWidget(Qt::BottomDockWidgetArea, this->log);

	this->mnNode = new Node("MN - 240");
	this->ui.statusbar->addPermanentWidget(this->mnNode);
}

MainWindow::~MainWindow()
{
	delete this->sdoTab;
	delete this->log;
	delete this->piVar;
	delete this->piMemory;
	if (!this->nwInterfaceDialog)
		delete this->nwInterfaceDialog;
	if (!this->cdcDialog)
		delete this->cdcDialog;
	delete this->nmtCmdWindow;
	delete this->cnStatus;
	delete this->mnNode;
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
	this->cdcDialog = new DialogOpenCdc(this);
	this->cdcDialog->exec();
}

void MainWindow::on_actionQuit_triggered()
{
	this->on_actionStop_triggered();
	this->close();
}

void MainWindow::on_actionSelect_Interface_triggered()
{
	this->nwInterfaceDialog = new SelectNwInterfaceDialog();
	if (this->nwInterfaceDialog->fillList() < 0)
	{
		QMessageBox::warning(this, "PCAP not working!",
							 "No PCAP interfaces found!\n"
							 "Make sure LibPcap is installed and you have root permissions!",
							 QMessageBox::Close);
		return;
	}

	if (this->nwInterfaceDialog->exec() == QDialog::Rejected)
	{
		return;
	}
	qDebug("Nw Name = %s ", qPrintable(this->nwInterfaceDialog->getDevName()));
}

void MainWindow::on_actionStart_triggered()
{
	//TODO Start powerlink and only if success enable the stop button.
	if ((this->nwInterfaceDialog == NULL)
		|| (this->nwInterfaceDialog->getDevName() == ""))
	{
		this->on_actionSelect_Interface_triggered();
	}
	this->ui.actionStop->setEnabled(true);
	this->ui.actionRestart->setEnabled(true);
	this->ui.actionStart->setEnabled(false);
	this->ui.statusbar->showMessage("asdfaskdasfsdfksdf");
	//this->mnNode->SetNodeStatus(0);
}

void MainWindow::on_actionStop_triggered()
{
	this->ui.actionStart->setEnabled(true);
	this->ui.actionRestart->setEnabled(false);
	this->ui.actionStop->setEnabled(false);
}

void MainWindow::on_actionRestart_triggered()
{

}

void MainWindow::on_actionAbout_triggered()
{

}

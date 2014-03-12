#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "ui_MainWindow.h"
#include "SdoTransfer.h"
#include "LoggerWindow.h"
#include "ProcessImageVariables.h"
#include "ProcessImageMemory.h"
#include "DialogOpenCdc.h"
#include "SelectNwInterfaceDialog.h"
#include "NmtCommandsDock.h"
#include "NodeStatusDock.h"
#include "NodeUi.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void on_actionToggle_Full_Screen_triggered();

	void on_actionOpen_CDC_triggered();

	void on_actionQuit_triggered();

	bool on_actionSelect_Interface_triggered();

	void on_actionStart_triggered();

	void on_actionStop_triggered();

	void on_actionRestart_triggered();

	void on_actionAbout_triggered();

	void on_actionHelp_triggered();

private:
	Ui::MainWindow ui;
	SdoTransfer *sdoTab; /// SDO ui
	LoggerWindow *log; /// Logging window
	ProcessImageVariables *piVar; /// Pi variable view
	ProcessImageMemory *piMemory; /// Pi memory view
	DialogOpenCdc *cdcDialog;  /// CDC dialog window
	SelectNwInterfaceDialog *nwInterfaceDialog; /// Network select interface dialog
	NmtCommandsDock *nmtCmdWindow; /// NMT command
	NodeStatusDock *cnStatus; /// CN status list
	NodeUi *mnNode; /// MN status frame
	// Q_DISABLE_COPY(MainWindow)
};

#endif // _MAINWINDOW_H_

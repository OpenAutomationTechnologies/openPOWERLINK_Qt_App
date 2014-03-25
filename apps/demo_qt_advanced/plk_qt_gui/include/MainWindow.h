/**
********************************************************************************
\file   MainWindow.h

\brief

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

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
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

#include "DataSyncThread.h"

#include "user/processimage/ProcessImageParser.h"


/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * @brief MainWindow
	 * @param parent
	 */
	explicit MainWindow(QWidget *parent = 0);

	/**
	 *
	 *
	 */
	~MainWindow();

private slots:

	/**
	 * @brief on_actionToggle_Full_Screen_triggered
	 */
	void on_actionToggle_Full_Screen_triggered();

	/**
	 * @brief on_actionOpen_CDC_triggered
	 */
	void on_actionOpen_CDC_triggered();

	/**
	 * @brief on_actionQuit_triggered
	 */
	void on_actionQuit_triggered();

	/**
	 * @brief on_actionSelect_Interface_triggered
	 * @return
	 */
	bool on_actionSelect_Interface_triggered();

	/**
	 * @brief on_actionStart_triggered
	 */
	void on_actionStart_triggered();

	/**
	 * @brief on_actionStop_triggered
	 */
	void on_actionStop_triggered();

	/**
	 * @brief on_actionRestart_triggered
	 */
	void on_actionRestart_triggered();

	/**
	 * @brief on_actionAbout_triggered
	 */
	void on_actionAbout_triggered();

	/**
	 * @brief on_actionHelp_triggered
	 */
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
	ProcessImageParser *parser;
	DataSyncThread *dataSync;
	// Q_DISABLE_COPY(MainWindow)
};

#endif // _MAINWINDOW_H_

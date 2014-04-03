/**
********************************************************************************
\file   MainWindow.h

\brief  Contains the actions and methods for the mainwindow GUI layout by
		using the Qt 5.2 QMainWindow.

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

#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/

#include "user/processimage/ProcessImageParser.h"

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

/**
 * \brief The MainWindow class inherits the QMainWindow and implements
 * the actions on it.
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	explicit MainWindow(QWidget *parent = 0);

	~MainWindow();

private slots:

	/**
	 * \brief Toggles the state of the window between fullscreen and normal mode.
	 * \note Full screen can also be triggered by pressing F11 keyboard shortcut.
	 */
	void on_actionToggle_Full_Screen_triggered();

	/**
	 * \brief Triggers displaying of the open CDC dialog window.
	 */
	void on_actionOpen_CDC_triggered();

	/**
	 * \brief Triggers the closing of the application
	 */
	void on_actionQuit_triggered();

	/**
	 * \brief Triggers displaying of PCAP select network interface dialog.
	 *
	 * \retval true if selected any network interface.
	 * \retval false if no network interface is selected.
	 */
	bool on_actionSelect_Interface_triggered();

	/**
	 * \brief Triggers start of the oplk stack.
	 *
	 * Imports the xap.xml, displays the necessary GUI and starts the oplk stack.
	 */
	void on_actionStart_triggered();

	/**
	 * \brief Triggers stop of the oplk stack.
	 *
	 * Clean-up all the memory allocated by the stack and removes the gui
	 * elements and stops the oplk stack.
	 */
	void on_actionStop_triggered();

	/**
	 * \brief Displays the about/info window.
	 */
	void on_actionAbout_triggered();

	/**
	 * \brief Triggers the actions linked to the help menu.
	 */
	void on_actionHelp_triggered();

private:
	Ui::MainWindow ui;                         ///< MainWindow UI instance
	SdoTransfer *sdoTab;                       ///< SDO ui
	LoggerWindow *log;                         ///< Logging window
	ProcessImageVariables *piVar;              ///< Pi variable view
	ProcessImageMemory *piMemory;              ///< Pi memory view
	DialogOpenCdc *cdcDialog;                  ///< CDC dialog window
	SelectNwInterfaceDialog *networkInterface; ///< Network select interface dialog
	NmtCommandsDock *nmtCmdWindow;             ///< NMT command
	NodeStatusDock *cnStatus;                  ///< CN status list
	ProcessImageParser *parser;                ///< ProcessImage xml Parser instance
	DataSyncThread *dataSync;                  ///< ProcessImageDataSync Thread
};

#endif // _MAIN_WINDOW_H_

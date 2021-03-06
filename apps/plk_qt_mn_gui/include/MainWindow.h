/**
********************************************************************************
\file   MainWindow.h

\brief  Contains the actions and methods for the main window GUI layout by
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
#include "ui_MainWindow.h"
#include "user/processimage/ProcessImageParser.h"

class SdoTransfer;
class LoggerWindow;
class ProcessImageVariables;
class ProcessImageMemory;
class ImportConfigurationDialog;
class NetworkInterfaceDialog;
class NmtCommandsDock;
class NodeStatusDock;
class StatusBar;

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

signals:
	/**
	 * \brief Signals once the stop stack has been triggered.
	 */
	void SignalStackStopped();

private slots:

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
	 * \brief Triggers start of the OPlk stack.
	 *
	 * Imports the xap.xml, displays the necessary GUI and starts the OPlk stack.
	 */
	void on_actionStart_triggered();

	/**
	 * \brief Triggers stop of the OPlk stack.
	 *
	 * Clean-up all the memory allocated by the stack and removes the GUI
	 * elements and stops the OPlk stack.
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

	/**
	 * \brief Handles the critical error events from the stack.
	 * \param[in] errorMessage Detailed information about the error.
	 */
	void HandleCriticalError(const QString& errorMessage);

private:
	Ui::MainWindow ui;                         ///< MainWindow UI instance
	SdoTransfer *sdo;                          ///< SDO user interface
	LoggerWindow *log;                         ///< Logging window
	ProcessImageVariables *piVar;              ///< Processimage variables view
	ProcessImageMemory *piMemory;              ///< Processimage memory view
	ImportConfigurationDialog *cdcDialog;      ///< CDC dialog window
	NetworkInterfaceDialog *networkInterface;  ///< Network select interface dialog
	NmtCommandsDock *nmtCmd;                   ///< NMT command
	NodeStatusDock *nodeStatus;                ///< CN status list
	ProcessImageParser *parser;                ///< ProcessImage xml Parser instance
	StatusBar *status;                         ///< Status bar

	std::string cdc;                           ///< Cdc file. Set after it is
											   /// selected from the ImportConfigurationDialog.
};

#endif // _MAIN_WINDOW_H_

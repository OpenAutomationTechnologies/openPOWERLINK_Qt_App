/**
********************************************************************************
\file   LoggerWindow.h

\brief  Refer to LoggerWindow

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

#ifndef _LOGGER_WINDOW_H_
#define _LOGGER_WINDOW_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/

#include "ui_LoggerWindow.h"

/**
 * \brief The LoggerWindow class inherits QDockWidget and updates the log
 * messages from various parts of the application.
 */
class LoggerWindow : public QDockWidget
{
	Q_OBJECT

public:
	explicit LoggerWindow(QWidget *parent = 0);

	/**
	 * \brief Handles the log signals from the stack.
	 *
	 *  Receives the log messages from the stack and appends to the stack log.
	 *
	 * \param[in] str The log string to be appended to the logwindow.
	 **/
	Q_INVOKABLE void HandleStackLog(const QString& str);

private slots:
	/**
	 * \brief Copies the contents of the log window.
	 */
	void on_copyStack_clicked();

	/**
	 * \brief Checks for if any text in the console is selected or not
	 * \param[in] txtSelected Checks if the text selected or not.
	 */
	void on_log_copyAvailable(bool txtSelected);

	/**
	 * \brief Clears the log.
	 */
	void on_clearStack_clicked();

private:
	Ui::LoggerWindow ui;  ///< Logger window ui instance.
	bool copyAvailable;   ///< Used to check if any of the log contents selected or not.
};

#endif // _LOGGER_WINDOW_H_

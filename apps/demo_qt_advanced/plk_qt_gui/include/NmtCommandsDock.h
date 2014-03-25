/**
********************************************************************************
\file   NmtCommandsDock.h

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

#ifndef _NMT_COMMANDS_DOCK_H_
#define _NMT_COMMANDS_DOCK_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "ui_NmtCommandsDock.h"
#include "user/nmtu.h"

/**
 * @brief The NmtCommandsDock class
 */
class NmtCommandsDock : public QDockWidget
{
	Q_OBJECT

public:
	/**
	 * @brief NmtCommandsDock
	 * @param parent
	 */
	explicit NmtCommandsDock(QWidget *parent = 0);

private slots:
	/**
	 * @brief on_sendNmtBtn_clicked
	 */
	void on_sendNmtBtn_clicked();

	/**
	 * @brief on_nmtCommand_currentIndexChanged
	 * @param index
	 */
	void on_nmtCommand_currentIndexChanged(int index);

private:
	Ui::NmtCommandsDock ui;
	tNmtCommand nmtCommand;
	// enum nmtCommand;
	// Q_DISABLE_COPY(NmtCommandsDock)
};

#endif // _NMT_COMMANDS_DOCK_H_

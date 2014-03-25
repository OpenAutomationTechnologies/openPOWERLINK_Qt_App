/**
********************************************************************************
\file   NmtCommandsDock.h

\brief  NmtCommandsDock uses the Qt 5.2 QDockWidget to provide the possibility
		for the user to send any possible NMT command to the any node in
		the network.

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
#include "user/nmtu.h"

#include "ui_NmtCommandsDock.h"

/**
 * \brief The NmtCommandsDock class inherits QDockWidget and provides the possibility
 * for the user to send any possible NMT command to the any node in the network.
 */
class NmtCommandsDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit NmtCommandsDock(QWidget *parent = 0);

private slots:
	/**
	 * \brief Triggers the NMT command to the stack.
	 */
	void on_sendNmtBtn_clicked();

	/**
	 * \brief Updates the equivalent NMT command whenever the selection has been changed.
	 * \param[in] index Chosen index of the NMT command dropdown
	 */
	void on_nmtCommand_currentIndexChanged(int index);

private:
	Ui::NmtCommandsDock ui; ///< NMT command ui instance
	tNmtCommand nmtCommand; ///< currently selected NMT command
	///It can also be done by having a map of nmt command.
};

#endif // _NMT_COMMANDS_DOCK_H_

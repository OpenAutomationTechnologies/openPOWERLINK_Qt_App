/**
********************************************************************************
\file   NmtCommandsDock.cpp

\brief  Implements the NmtCommandDock class using the oplkQtApi library.

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
#include "NmtCommandsDock.h"

#include <QMessageBox>
#include "api/OplkQtApi.h"
#include "oplk/debugstr.h"

/*******************************************************************************
* Public functions
*******************************************************************************/
NmtCommandsDock::NmtCommandsDock(QWidget *parent) :
	QDockWidget(parent)
{
	this->ui.setupUi(this);
}


/*******************************************************************************
* Private functions
*******************************************************************************/
void NmtCommandsDock::on_sendNmtBtn_clicked()
{
	//Send NMT command is not working for MN.
	// TODO in API library.
	const int nodeid = this->ui.nodeId->text().toInt(0, 16);
	qDebug("%d", nodeid);
	tOplkError oplkRet = OplkQtApi::ExecuteNmtCommand(nodeid, this->nmtCommand);
	if (oplkRet != kErrorOk)
	{
		QMessageBox::critical(this, "Execute NmtCommand failed",
							 QString("Execute NmtCommand falied with error: %1 ")
							  .arg(debugstr_getRetValStr(oplkRet)),
							 QMessageBox::Close);
		qDebug("Execute NmtCommand %x", oplkRet);
		return;
	}
}

/**
 * This can also be done as
 * A function to get the NMT command by passing the CurrentIndex of the
 * NMT dropdown at the time of the send NMT button clicked.
 * signature: tNmtCommand GetNmtCommand(int currentIndex);
 */

void NmtCommandsDock::on_nmtCommand_currentIndexChanged(int index)
{
	/* It can be also done by parsing the string and getting the 0xXX
	 * code to pass for the NMT command enum
	 */

	switch (index)
	{
		//TODO fallthrough case 0 and default.
		case 0:
			this->nmtCommand = kNmtCmdInvalidService;
			break;
		case 1:
			this->nmtCommand = kNmtCmdStartNode;
			break;
		case 2:
			this->nmtCommand = kNmtCmdStopNode;
			break;
		case 3:
			this->nmtCommand = kNmtCmdEnterPreOperational2;
			break;
		case 4:
			this->nmtCommand = kNmtCmdEnableReadyToOperate;
			break;
		case 5:
			this->nmtCommand = kNmtCmdResetNode;
			break;
		case 6:
			this->nmtCommand = kNmtCmdResetCommunication;
			break;
		case 7:
			this->nmtCommand = kNmtCmdResetConfiguration;
			break;
		case 8:
			this->nmtCommand = kNmtCmdSwReset;
			break;
		default:
			this->nmtCommand = kNmtCmdInvalidService;
			break;
	}
}


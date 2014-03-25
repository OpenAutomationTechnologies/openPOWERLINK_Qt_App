 /**
********************************************************************************
\file   NodeStatusDock.h

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

#ifndef _NODESTATUSDOCK_H_
#define _NODESTATUSDOCK_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <QList>

#include "ui_NodeStatusDock.h"
#include "NodeUi.h"

class NodeStatusDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit NodeStatusDock(QWidget *parent = 0);
	// NodeStatusDock& GetInstance();
	QStringList GetAvailableCnList();
	/**
	 * \brief   Handles the Node State changed signals
	 *
	 * \param[in] nodeId	nodeId of the node which changes the state.
	 * \param[in] nmtState	New state of the node.
	 */
	Q_INVOKABLE void HandleNodeStateChanged(const int nodeId, tNmtState nmtState);
	Q_INVOKABLE void HandleNodeFound(const int nodeId);
	Q_INVOKABLE void HandleMnStateChanged(tNmtState nmtState);

private:
	Ui::NodeStatusDock ui;
	// TODO replace with QList
	// NodeUi **nodelist;
	QList<NodeUi*> nodelists;
	// May be we can use QMap
	// QMap<const uint, NodeUi*> nodelist;
	// Q_DISABLE_COPY(NodeStatusDock)
};

#endif // _NODESTATUSDOCK_H_

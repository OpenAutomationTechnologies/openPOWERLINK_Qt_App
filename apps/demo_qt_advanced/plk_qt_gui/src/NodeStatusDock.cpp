/**
********************************************************************************
\file   NodeStatusDock.cpp

\brief  Implements the node status dock and updates the status of the available
		CN node id's.

\todo   Rename to Network status Dock.

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
#include "NodeStatusDock.h"
#include "api/OplkQtApi.h"

#include <oplk/debugstr.h>

const UINT kMaxNodes = 240;
//TODO have nodeId in common place.
const UINT kmnNodeId = 240;

NodeStatusDock::NodeStatusDock(QWidget *parent) :
	QDockWidget(parent)
{
	this->ui.setupUi(this);

	// this->ui.verticalLayout->setDirection(QBoxLayout::Down);
	this->ui.verticalLayout->setAlignment(Qt::AlignTop);

	NodeUi *nodeUi = NULL;
	for (UINT i = 0; i < kMaxNodes; ++i)
	{
		if (i == 0)
		{
			nodeUi = new NodeUi(kmnNodeId);
		}
		else
		{
			nodeUi = new NodeUi(i);
			nodeUi->hide();
		}
		this->nodelists.push_back(nodeUi);
		this->ui.verticalLayout->addWidget(nodeUi);
	}

	int index = this->metaObject()->indexOfMethod(
					QMetaObject::normalizedSignature(
					"HandleNodeStateChanged(const int, tNmtState)").constData());
	Q_ASSERT(index != -1);
	// If asserted check for the Function name

	bool ret = OplkQtApi::RegisterNodeStateChangedEventHandler(*(this),
							this->metaObject()->method(index));
	Q_ASSERT(ret != false);

	index = this->metaObject()->indexOfMethod(
					QMetaObject::normalizedSignature(
					"HandleNodeFound(const int)").constData());
	Q_ASSERT(index != -1);
	// If asserted check for the Function name

	ret = OplkQtApi::RegisterNodeFoundEventHandler(*(this),
							this->metaObject()->method(index));
	Q_ASSERT(ret != false);

	index = this->metaObject()->indexOfMethod(
					QMetaObject::normalizedSignature(
						"HandleMnStateChanged(tNmtState)").constData());
	Q_ASSERT(index != -1);
	// If asserted check for the Function name

	ret = OplkQtApi::RegisterLocalNodeStateChangedEventHandler(*(this),
							this->metaObject()->method(index));
	Q_ASSERT(ret != false);
}

void NodeStatusDock::HandleMnStateChanged(tNmtState nmtState)
{
	//At (0) refers to the MN index.
	this->nodelists.at(0)->HandleNodeStateChanged(nmtState);

	// Change all CN states for Stack shutdown / MN dead.
	if ((nmtState == kNmtMsBasicEthernet) || (nmtState < kNmtMsOperational))
	{
		for (QList<NodeUi*>::iterator it = this->nodelists.begin();
			 it != this->nodelists.end(); ++it)
		{
			if ((*it)->GetNodeId() != kmnNodeId)
			{
				(*it)->hide();
				emit SignalNodeNotActive((*it)->GetNodeId());
			}
		}
	}

	if (nmtState == kNmtMsOperational)
		emit SignalNodeAvailable(kmnNodeId);
	else
		emit SignalNodeNotActive(kmnNodeId);
}

void NodeStatusDock::HandleNodeStateChanged(const int nodeId, tNmtState nmtState)
{
	// qDebug("HandleN %d %s", nodeId, debugstr_getNmtStateStr(nmtState));
	if ((nodeId > 0) && (nodeId <= kMaxNodes))
	{
		if ((this->nodelists.at(nodeId)))
		{
			this->nodelists.at(nodeId)->HandleNodeStateChanged(nmtState);
			// Hide CN for Loss of PRes
			if (nmtState == kNmtCsNotActive)
			{
				this->nodelists.at(nodeId)->hide();
			}
			else
			{
				// If this is not done CN will not show up even in OPERATIONAL
				this->HandleNodeFound(nodeId);
			}
		}

		if (nmtState == kNmtCsOperational)
			emit SignalNodeAvailable(nodeId);
		else// (nmtState == KNmtCsNotActive)
			emit SignalNodeNotActive(nodeId);
	}
}

void NodeStatusDock::HandleNodeFound(const int nodeId)
{
	// qDebug(" F %d ", nodeId);
	if ((nodeId > 0) && (nodeId <= kMaxNodes))
	{
		if (this->nodelists.at(nodeId))
		{
			this->nodelists.at(nodeId)->show();
		}
	}
}

/**
********************************************************************************
\file   NodeUi.cpp

\brief  Implements the NodeUi frame.

\todo
		- It can have a progressbar to display the CFM download status.

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
#include "NodeUi.h"
#include <QPainter>
#include <QPen>
#include <QBrush>

#include <oplk/debugstr.h>

const QString NodeUi::ledRed = ":/new/images/red.png";
const QString NodeUi::ledGreen = ":/new/images/green.png";
const QString NodeUi::ledBlue = ":/new/images/blue.png";
const QString NodeUi::ledYellow = ":/new/images/yellow.png";
const QString NodeUi::ledBrown = ":/new/images/brown.png";
const QString NodeUi::ledWhite = ":/new/images/white.png";
const QString NodeUi::ledGrey = ":/new/images/grey.png";

/*******************************************************************************
* Public functions
*******************************************************************************/
NodeUi::NodeUi(const UINT nodeId, QWidget *parent) :
	QFrame(parent),
	nodeId(nodeId),
	nodeLayout(new QHBoxLayout(this)),
	name(new QLabel()),
	statusImage(new QLabel())
{
//	this->setMaximumSize(QSize(200, 47));

/// Setting node name
	if (this->nodeId == 240)
	{
		this->name->setText(QString("   MN-%1").arg(this->nodeId));
		this->name->setMinimumWidth(20);
		this->name->setMaximumWidth(80);
		this->name->setFont(QFont("Arial", 11, QFont::Bold));
	}
	else
	{
		this->name->setText(QString("          CN-%1 ").arg(this->nodeId));
		this->name->setMinimumWidth(110);
		this->name->setFont(QFont("Arial", 11, QFont::Normal));
	}
	this->nodeLayout->addWidget(this->name);

	this->statusImage->setPixmap(QPixmap(NodeUi::ledRed));
	this->nodeLayout->addWidget(this->statusImage);

	this->setToolTip(this->name->text());

	this->nodeLayout->update();
}

const UINT NodeUi::GetNodeId() const
{
	return this->nodeId;
}

void NodeUi::HandleNodeStateChanged(tNmtState nmtState)
{
	this->setToolTip((QString("%1 : %2")
					.arg(this->name->text())
					.arg(debugstr_getNmtStateStr(nmtState)))
					.trimmed());

	QPixmap image;
	switch (nmtState)
	{
		case kNmtMsNotActive:
		case kNmtCsNotActive:
		{
			image = QPixmap(NodeUi::ledGrey);
			break;
		}
		case kNmtMsPreOperational1:
		case kNmtMsPreOperational2:
		case kNmtCsPreOperational1:
		case kNmtCsPreOperational2:
		{
			image = QPixmap(NodeUi::ledYellow);
			break;
		}
		case kNmtGsOff:
		case kNmtCsStopped:
		{
			image = QPixmap(NodeUi::ledRed);
			break;
		}
		case kNmtMsReadyToOperate:
		case kNmtCsReadyToOperate:
		{
			image = QPixmap(NodeUi::ledBrown);
			break;
		}
		case kNmtMsOperational:
		case kNmtCsOperational:
		{
			image = QPixmap(NodeUi::ledGreen);
			break;
		}
		case kNmtMsBasicEthernet:
		case kNmtCsBasicEthernet:
		{
			image = QPixmap(NodeUi::ledBlue);
			break;
		}
		case kNmtGsInitialising:
		case kNmtGsResetApplication:
		case kNmtGsResetCommunication:
		case kNmtGsResetConfiguration:
		default:
			image = QPixmap(NodeUi::ledWhite);
			break;
	}
	this->statusImage->setPixmap(image);
	this->nodeLayout->update();
}

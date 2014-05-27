/**
********************************************************************************
\file   NodeWidget.cpp

\brief  Implements the NodeWidget.

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
#include "NodeWidget.h"
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtGui/QBrush>

#include <oplk/debugstr.h>

const QString NodeWidget::ledRed = ":/new/images/red.png";
const QString NodeWidget::ledGreen = ":/new/images/green.png";
const QString NodeWidget::ledBlue = ":/new/images/blue.png";
const QString NodeWidget::ledYellow = ":/new/images/yellow.png";
const QString NodeWidget::ledBrown = ":/new/images/brown.png";
const QString NodeWidget::ledWhite = ":/new/images/white.png";
const QString NodeWidget::ledGrey = ":/new/images/grey.png";

/*******************************************************************************
* Public functions
*******************************************************************************/
NodeWidget::NodeWidget(const UINT nodeId, QWidget *parent) :
	QWidget(parent),
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

	this->statusImage->setPixmap(QPixmap(NodeWidget::ledRed));
	this->nodeLayout->addWidget(this->statusImage);

	this->setToolTip(this->name->text());

	this->nodeLayout->update();
}

const UINT NodeWidget::GetNodeId() const
{
	return this->nodeId;
}

void NodeWidget::HandleNodeStateChanged(tNmtState nmtState)
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
			image = QPixmap(NodeWidget::ledGrey);
			break;
		}
		case kNmtMsPreOperational1:
		case kNmtMsPreOperational2:
		case kNmtCsPreOperational1:
		case kNmtCsPreOperational2:
		{
			image = QPixmap(NodeWidget::ledYellow);
			break;
		}
		case kNmtGsOff:
		case kNmtCsStopped:
		{
			image = QPixmap(NodeWidget::ledRed);
			break;
		}
		case kNmtMsReadyToOperate:
		case kNmtCsReadyToOperate:
		{
			image = QPixmap(NodeWidget::ledBrown);
			break;
		}
		case kNmtMsOperational:
		case kNmtCsOperational:
		{
			image = QPixmap(NodeWidget::ledGreen);
			break;
		}
		case kNmtMsBasicEthernet:
		case kNmtCsBasicEthernet:
		{
			image = QPixmap(NodeWidget::ledBlue);
			break;
		}
		case kNmtGsInitialising:
		case kNmtGsResetApplication:
		case kNmtGsResetCommunication:
		case kNmtGsResetConfiguration:
		default:
			image = QPixmap(NodeWidget::ledWhite);
			break;
	}
	this->statusImage->setPixmap(image);
	this->nodeLayout->update();
}

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

/*******************************************************************************
* Public functions
*******************************************************************************/
NodeUi::NodeUi(const UINT nodeId, QWidget *parent) :
	QFrame(parent),
	nodeId(nodeId),
	nodeLayout(new QHBoxLayout(this)),
	name(new QLabel()),
	statusImage(new QLabel()),
	statusPixmap(QPixmap(QSize(27, 27)))
{
	this->setMaximumSize(QSize(200, 45));

/// Setting node name
	if (this->nodeId == 240)
	{
		this->name->setText(QString("MN-%1 ").arg(this->nodeId));
		this->name->setMinimumWidth(75);
		this->name->setFont(QFont("Arial", 11, QFont::Bold));
	}
	else
	{
		this->name->setText(QString("      CN-%1 ").arg(this->nodeId));
		this->name->setMinimumWidth(100);
		this->name->setFont(QFont("Arial", 10, QFont::Bold));
	}
	this->nodeLayout->addWidget(this->name);


	this->statusPixmap.fill(Qt::transparent);

	QPainter painter(&(this->statusPixmap));
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(QPen(Qt::transparent));
	painter.setBrush(QBrush(Qt::red));
	painter.drawEllipse(0, 0, 25, 25);

	this->statusImage->setPixmap(this->statusPixmap);
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

	QPainter painter(&(this->statusPixmap));
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(QPen(Qt::transparent));

	// can also use QBrush(const QGradient & gradient)
	switch (nmtState)
	{
		case kNmtMsNotActive:
		case kNmtCsNotActive:
		{
			painter.setBrush(QBrush(Qt::gray));
			break;
		}
		case kNmtMsPreOperational1:
		case kNmtMsPreOperational2:
		case kNmtCsPreOperational1:
		case kNmtCsPreOperational2:
		{
			painter.setBrush(QBrush(Qt::yellow));
			break;
		}
		case kNmtGsOff:
		case kNmtCsStopped:
		{
			painter.setBrush(QBrush(Qt::red));
			break;
		}
		case kNmtMsReadyToOperate:
		case kNmtCsReadyToOperate:
		{
			painter.setBrush(QColor(255, 111, 0));
			//painter.setBrush(QBrush(Qt::magenta));
			break;
		}
		case kNmtMsOperational:
		case kNmtCsOperational:
		{
			painter.setBrush(QBrush(Qt::green));
			break;
		}
		case kNmtMsBasicEthernet:
		case kNmtCsBasicEthernet:
		{
			painter.setBrush(QBrush(Qt::blue));
			break;
		}
		case kNmtGsInitialising:
		case kNmtGsResetApplication:
		case kNmtGsResetCommunication:
		case kNmtGsResetConfiguration:
		default:
			painter.setBrush(QBrush(Qt::white));
			break;
	}
	painter.drawEllipse(0, 0, 25, 25);
	this->statusImage->setPixmap(this->statusPixmap);
	this->nodeLayout->update();
}

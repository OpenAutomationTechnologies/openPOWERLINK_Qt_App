/**
********************************************************************************
\file   StatusBar.cpp

\brief  Implements the status bar and its labels.

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
#include "StatusBar.h"

#include "api/OplkQtApi.h"
#include "oplk/debugstr.h"

StatusBar::StatusBar(QWidget *parent) :
	QStatusBar(parent),
	networkInterface(new QLabel("Network Interface")),
	cdcFile(new QLabel("CDC File")),
	xapFile(new QLabel("Xap File")),
	nmtStatus(new QLabel("NMT Status")),
	cycleTime(new QLabel("Cycle Time"))
{
	this->networkInterface->setMinimumWidth(175);
	this->cdcFile->setMinimumWidth(175);
	this->xapFile->setMinimumWidth(175);

	this->addWidget(this->networkInterface);
	this->addWidget(this->cdcFile);
	this->addWidget(this->xapFile);

	QFont font;
	font.setCapitalization(QFont::AllUppercase);
	this->nmtStatus->setFont(font);

	this->nmtStatus->setMinimumWidth(100);
	this->cycleTime->setMinimumWidth(100);

	this->addPermanentWidget(this->nmtStatus);
	this->addPermanentWidget(this->cycleTime);
	this->addPermanentWidget(new QLabel("   "));
}

void StatusBar::UpdateNmtStatus(tNmtState nmtState)
{
	QString debugState = debugstr_getNmtStateStr(nmtState);

	QString state;
	bool firstIndex = true;

	foreach (const QChar &ch, debugState)
	{
		if (ch.isLetter() && ch.isUpper())
		{
			if (firstIndex)
				firstIndex = !firstIndex;
			else
				state += "_";
		}
		state += ch;
	}

	qDebug("%s", qPrintable(state));
	this->nmtStatus->setText(state);

	if (nmtState == kNmtMsPreOperational1)
		this->UpdateCycleTime();
}

void StatusBar::UpdateCycleTime()
{
	DWORD cycleTime;
	UINT size = sizeof (cycleTime);
	tOplkError oplkRet = oplk_readLocalObject(0x1006,
											  0x00,
											  (void*)(&cycleTime),
											  &size);
	//TODO Assert if err
	if (oplkRet != kErrorOk)
		qDebug("Cycle time read error: %s", debugstr_getRetValStr(oplkRet));

	this->cycleTime->setText(QString("CycleTime(us)=%1").arg(cycleTime));
}

void StatusBar::SetCdcFilePath(const char* cdc)
{
	this->cdcFile->setToolTip(QString::fromUtf8(cdc));
	this->cdcFile->setText(this->GetShortPath(QString::fromUtf8(cdc)));
}

void StatusBar::SetXapFilePath(const char* xap)
{
	this->xapFile->setToolTip(QString::fromUtf8(xap));
	this->xapFile->setText(this->GetShortPath(QString::fromUtf8(xap)));
}

void StatusBar::SetNetworkInterfaceName(const QString name)
{
	const QString devName = name.simplified();
	this->networkInterface->setToolTip(devName);
	this->networkInterface->setText(devName.left(25));
}

//can be moved to Utility
const QString StatusBar::GetShortPath(const QString& path) const
{
	QString shortPath;
	QStringList splitCDC = path.split("/", QString::SkipEmptyParts);
	bool fillersAdded = false;
	for (UINT it = 0; it < splitCDC.size(); ++it)
	{
		if ((it < 2))
		{
			shortPath.append(splitCDC.at(it));
			shortPath.append("/");
		}
		else if ((it == (splitCDC.size() - 1)))
		{
			shortPath.append(splitCDC.at(it));
		}
		else
		{
			if (!fillersAdded)
			{
				shortPath.append(".../");
				fillersAdded = !fillersAdded;
			}
		}
	}
	return shortPath;
}


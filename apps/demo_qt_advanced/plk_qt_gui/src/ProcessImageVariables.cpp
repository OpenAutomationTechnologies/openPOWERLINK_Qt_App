/**
********************************************************************************
\file   ProcessImageVariables.cpp

\brief  Implements the variables view of the input and output processimage data
from the oplk stack.

\todo  This can also be done with QTableWidget to prepare the input and output tables.

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
#include "ProcessImageVariables.h"

#include <QtCore/QLocale>

#include "api/OplkQtApi.h"

/*******************************************************************************
* Public functions
*******************************************************************************/
ProcessImageVariables::ProcessImageVariables(QWidget *parent) :
	QWidget(parent),
	inPi(NULL),
	outPi(NULL),
	processImageInputSlotIndex(this->metaObject()->indexOfMethod(
								QMetaObject::normalizedSignature(
								"UpdateFromInputValues()").constData())),
	processImageOutputSlotIndex(this->metaObject()->indexOfMethod(
								QMetaObject::normalizedSignature(
								"UpdateFromOutputValues()").constData()))
{
	this->ui.setupUi(this);
	Q_ASSERT(processImageInputSlotIndex != -1);
	Q_ASSERT(processImageOutputSlotIndex != -1);
}

void ProcessImageVariables::ResetView()
{
	//Unregister for ProcessImage variables input datas.
	bool ret = OplkQtApi::UnregisterSyncEventHandler(Direction::PI_IN,
										 *(this),
										 this->metaObject()->method(this->processImageInputSlotIndex));
	Q_ASSERT(ret != false);

	//Unregister for ProcessImage variables output datas.
	ret = OplkQtApi::UnregisterSyncEventHandler(Direction::PI_OUT,
										 *(this),
										 this->metaObject()->method(this->processImageOutputSlotIndex));
	Q_ASSERT(ret != false);

// clear ui once stack has stopped.
	for (QList<ChannelUi*>::iterator channel = this->inputChannels.begin();
		 channel != this->inputChannels.end(); ++channel)
	{
		if (*channel)
		{
			this->ui.inputProcessImage->removeWidget(*channel);
			delete (*channel);
		}
	}

	this->inputChannels.clear();

	for (QList<ChannelUi*>::iterator channel = this->outputChannels.begin();
			 channel != this->outputChannels.end(); ++channel)
	{
		if (*channel)
		{
			this->ui.outputProcessImage->removeWidget(*channel);
			delete (*channel);
		}
	}

	this->outputChannels.clear();

	this->ui.inputProcessImage->update();
	this->ui.outputProcessImage->update();
}

ProcessImageVariables::~ProcessImageVariables()
{
	for (QList<ChannelUi*>::iterator channel = this->inputChannels.begin();
		 channel != this->inputChannels.end(); ++channel)
	{
		if (*channel)
		{
			delete (*channel);
		}
	}
	this->inputChannels.clear();

	for (QList<ChannelUi*>::iterator channel = this->outputChannels.begin();
		 channel != this->outputChannels.end(); ++channel)
	{
		if (*channel)
		{
			delete (*channel);
		}
	}
	this->outputChannels.clear();
}

void ProcessImageVariables::SetProcessImage(ProcessImageIn *inPi, const ProcessImageOut *outPi)
{
	this->inPi = inPi;
	if (inPi)
	{
		this->PrepareInputRows();

		//Register for ProcessImage variables input datas.
		bool ret = OplkQtApi::RegisterSyncEventHandler(Direction::PI_IN,
											 *(this),
											 this->metaObject()->method(this->processImageInputSlotIndex));
		Q_ASSERT(ret != false);
	}

	this->outPi = outPi;
	if (outPi)
	{
		this->PrepareOutputRows();
		//Register for ProcessImage variables output datas.
		bool ret = OplkQtApi::RegisterSyncEventHandler(Direction::PI_OUT,
											 *(this),
											 this->metaObject()->method(this->processImageOutputSlotIndex));
		Q_ASSERT(ret != false);
	}
}

void ProcessImageVariables::UpdateFromInputValues()
{
	for (QList<ChannelUi*>::iterator channel = this->inputChannels.begin();
		 channel != this->inputChannels.end(); ++channel)
	{
		if (*channel)
		{
			(*channel)->UpdateInputChannelCurrentValue(this->inPi);
		}
	}
}

void ProcessImageVariables::UpdateFromOutputValues()
{
	for (QList<ChannelUi*>::iterator channel = this->outputChannels.begin();
		 channel != this->outputChannels.end(); ++channel)
	{
		if (*channel)
		{
			(*channel)->UpdateOutputChannelCurrentValue(this->outPi);
		}
	}
}

/*******************************************************************************
* Private functions
*******************************************************************************/

void ProcessImageVariables::PrepareInputRows()
{
	ChannelUi *channel = NULL;
	for (std::map<std::string, Channel>::const_iterator it = this->inPi->cbegin();
		 it != this->inPi->cend(); ++it)
	{
		// qDebug(qPrintable(QString::fromStdString(it->first)));
		channel = new ChannelUi(it->second);
		this->inputChannels.push_back(channel);
		this->ui.inputProcessImage->addWidget(channel);
	}

	this->on_inputCheckAll_stateChanged(this->ui.inputCheckAll->checkState());
}

void ProcessImageVariables::PrepareOutputRows()
{
	ChannelUi *channel = NULL;
	for (std::map<std::string, Channel>::const_iterator it = this->outPi->cbegin();
		 it != this->outPi->cend(); ++it)
	{
		// qDebug(qPrintable(QString::fromStdString(it->first)));
		channel = new ChannelUi(it->second);
		this->outputChannels.push_back(channel);
		this->ui.outputProcessImage->addWidget(channel);
	}

	this->on_outputCheckAll_stateChanged(this->ui.outputCheckAll->checkState());
}

void ProcessImageVariables::on_inputCheckAll_stateChanged(int checkedState)
{
	if (checkedState != Qt::PartiallyChecked)
	{
		for (QList<ChannelUi*>::iterator channel = this->inputChannels.begin();
			 channel != this->inputChannels.end(); ++channel)
		{
			if (*channel)
			{
				(*channel)->UpdateSelectCheckBox((Qt::CheckState) checkedState);
			}
		}
	}
}

void ProcessImageVariables::on_outputCheckAll_stateChanged(int checkedState)
{
	if (checkedState != Qt::PartiallyChecked)
	{
		for (QList<ChannelUi*>::iterator channel = this->outputChannels.begin();
			 channel != this->outputChannels.end(); ++channel)
		{
			if (*channel)
			{
				(*channel)->UpdateSelectCheckBox((Qt::CheckState) checkedState);
			}
		}
	}
}

void ProcessImageVariables::on_inputHideCheckedBtn_clicked()
{
	for (QList<ChannelUi*>::iterator channel = this->inputChannels.begin();
		 channel != this->inputChannels.end(); ++channel)
	{
		if (*channel)
		{
			if ((*channel)->GetSelectCheckBoxState() == Qt::Checked)
			{
				(*channel)->hide();
			}
		}
	}
}

void ProcessImageVariables::on_inputShowAllBtn_clicked()
{
	for (QList<ChannelUi*>::iterator channel = this->inputChannels.begin();
		 channel != this->inputChannels.end(); ++channel)
	{
		if (*channel)
		{
			(*channel)->show();
		}
	}
}

void ProcessImageVariables::on_outHideCheckedBtn_clicked()
{
	for (QList<ChannelUi*>::iterator channel = this->outputChannels.begin();
		 channel != this->outputChannels.end(); ++channel)
	{
		if (*channel)
		{
			if ((*channel)->GetSelectCheckBoxState() == Qt::Checked)
			{
				(*channel)->hide();
			}
		}
	}
}

void ProcessImageVariables::on_outShowAllBtn_clicked()
{
	for (QList<ChannelUi*>::iterator channel = this->outputChannels.begin();
		 channel != this->outputChannels.end(); ++channel)
	{
		if (*channel)
		{
			(*channel)->show();
		}
	}
}


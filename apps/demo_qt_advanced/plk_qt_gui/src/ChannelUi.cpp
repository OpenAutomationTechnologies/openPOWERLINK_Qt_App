/**
********************************************************************************
\file   ChannelUi.cpp

\brief  Implements the actions handled with the channel.

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
#include "ChannelUi.h"
#include <QDebug>


/*******************************************************************************
* Public functions
*******************************************************************************/

ChannelUi::ChannelUi(const Channel &channel, QWidget *parent) :
	QWidget(parent),
	channel(channel),
	lockValueTexbox(false),
	input(NULL),
	value(new LineEditUi()),
	valueBeforeLock("")
{
	ui.setupUi(this);

	this->ui.channelName->setText(QString::fromStdString(this->channel.GetName()));

	//Create current value textbox
	value->setMinimumSize(QSize(95, 0));
	value->setMaximumSize(QSize(100, 16777215));
	value->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
	this->ui.horizontalLayout->addWidget(value);

	bool ret = QObject::connect(this->value,
								SIGNAL(SignalFocusIn()),
								this,
								SLOT(LockCurrentValue()));
	Q_ASSERT(ret != false);

	ret = QObject::connect(this->value,
						   SIGNAL(SignalFocusOut()),
						   this,
						   SLOT(UnlockCurrentValue()));
	Q_ASSERT(ret != false);

	ret = QObject::connect(this->value,
						   SIGNAL(returnPressed()),
						   this,
						   SLOT(ValueReturnPressed()));
	Q_ASSERT(ret != false);

	// Hide force check box and forcevalue text box for output PI
	if (this->channel.GetDirection() == Direction::PI_OUT)
	{
		this->value->setEnabled(false);
	}

	this->setToolTip(QString("Size = %1 bits \nByteOffset = 0x%2 \nBitOffset = 0x%3")
					 .arg(this->channel.GetBitSize())
					 .arg(this->channel.GetByteOffset(), 0, 16)
					 .arg(this->channel.GetBitOffset(), 0, 16));
	this->SetInputMask();
}

ChannelUi::~ChannelUi()
{
	delete this->ui.channelName;
	delete this->ui.check;
	delete this->value;
	delete this->ui.horizontalLayout;
}

void ChannelUi::UpdateSelectCheckBox(Qt::CheckState state)
{
	this->ui.check->setChecked(state);
}

Qt::CheckState ChannelUi::GetSelectCheckBoxState() const
{
	return this->ui.check->checkState();
}

void ChannelUi::UpdateInputChannelCurrentValue(ProcessImageIn *in)
{
	this->input = in;

	try
	{
		if (!this->lockValueTexbox)
		{
			std::vector<BYTE> value = in->GetRawData(this->channel.GetBitSize(),
													this->channel.GetByteOffset(),
													this->channel.GetBitOffset());
			QString string;
			for (std::vector<BYTE>::reverse_iterator it = value.rbegin();
					it != value.rend(); ++it )
			{
				string.append(QString("%1").arg(*it, 0, 16)).rightJustified(2, '0');
			}
			this->SetCurrentValue(string.toUpper());
		}
	}
	catch(const std::exception& ex)
	{
		// TODO Discuss about exposing the error to the user.
		qDebug("An Exception has occured: %s", ex.what());
	}
}

void ChannelUi::UpdateOutputChannelCurrentValue(const ProcessImageOut *out)
{
	try
	{
		std::vector<BYTE> value = out->GetRawData(this->channel.GetBitSize(),
												this->channel.GetByteOffset(),
												this->channel.GetBitOffset());
		QString string;
		for (std::vector<BYTE>::reverse_iterator it = value.rbegin();
				it != value.rend(); ++it )
		{
			string.append(QString("%1").arg(*it, 0, 16)).rightJustified(2, '0');
		}
		this->SetCurrentValue(string);
	}
	catch(const std::exception& ex)
	{
		// TODO Discuss about exposing the error to the user.
		qDebug("An Exception has occured: %s", ex.what());
	}
}

void ChannelUi::SetInputMask()
{
	if ((this->channel.GetBitSize() % 8) == 0)
	{
		QString inputMask;
		for (UINT i = 0; i < (this->channel.GetBitSize() / 4); ++i)
		{
			inputMask.append("H");
		}
		this->value->setInputMask(inputMask);
	}
	else
	{
		if (this->channel.GetBitSize() == 1)
		{
			this->value->setInputMask("B");
		}
		else
		{
			this->value->setInputMask("H");
		}
	}
}

void ChannelUi::SetCurrentValue(QString str)
{
	this->value->setText(str.toUpper());
}

/*******************************************************************************
* Private SLOTS
*******************************************************************************/

void ChannelUi::ValueReturnPressed()
{
	try
	{
		const QString forceValue = this->value->text();
		if (!forceValue.isEmpty()
			&& (forceValue.compare(this->valueBeforeLock, Qt::CaseInsensitive) != 0))
		{
			const qlonglong forc = forceValue.toLongLong(0, 16);

			this->input->SetRawValue(this->channel.GetName(),
							(const void*) &forc,
							this->channel.GetBitSize());
			this->setStyleSheet("QLineEdit{background: white;}");
		}
	}
	catch(const std::exception& ex)
	{
		// TODO Discuss about exposing the error to the user.
		qDebug("An Exception has occured: %s", ex.what());
	}
}

void ChannelUi::LockCurrentValue()
{
	this->valueBeforeLock = this->value->text();
	this->lockValueTexbox = true;
	this->setStyleSheet("QLineEdit{background: yellow;}");
}

void ChannelUi::UnlockCurrentValue()
{
	try
	{
		const QString forceValue = this->value->text();
		if (!forceValue.isEmpty()
			&& (forceValue.compare(this->valueBeforeLock, Qt::CaseInsensitive) != 0))
		{
			const qlonglong forc = forceValue.toLongLong(0, 16);

			this->input->SetRawValue(this->channel.GetName(),
							(const void*) &forc,
							this->channel.GetBitSize());
		}
	}
	catch(const std::exception& ex)
	{
		// TODO Discuss about exposing the error to the user.
		qDebug("An Exception has occured: %s", ex.what());
	}

	this->lockValueTexbox = false;
	this->setStyleSheet("QLineEdit{background: white;}");
}

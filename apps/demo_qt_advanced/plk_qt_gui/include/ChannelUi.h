/**
********************************************************************************
\file   ChannelUi.h

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

#ifndef _UI_CHANNEL_H_
#define _UI_CHANNEL_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/

#include "ui_ChannelUi.h"
#include "user/processimage/Channel.h"
#include "user/processimage/ProcessImageIn.h"
#include "user/processimage/ProcessImageOut.h"

/**
 * @brief The ChannelUi class
 */
class ChannelUi : public QWidget
{
	Q_OBJECT

public:
	/**
	 * @brief ChannelUi
	 * @param channel
	 * @param parent
	 */
	explicit ChannelUi(Channel channel, QWidget *parent = 0);

	/*
	 *
	 */
	~ChannelUi();

	/**
	 * @brief UpdateSelectCheckBox
	 * @param forceState
	 */
	void UpdateSelectCheckBox(Qt::CheckState forceState);

	/**
	 * @brief GetSelectCheckBoxState
	 * @return
	 */
	Qt::CheckState GetSelectCheckBoxState();

	/**
	 * @brief SetCurrentValue
	 * @param setStr
	 */
	void SetCurrentValue(QString setStr);

	/**
	 * @brief GetCurrentValue
	 * @return
	 */
	QString GetCurrentValue();

	/**
	 * @brief GetForceValue
	 * @return
	 */
	QString GetForceValue();

	/**
	 * @brief UpdateForceCheckBox
	 * @param forceState
	 */
	void UpdateForceCheckBox(Qt::CheckState forceState);

	/**
	 * @brief GetForceCheckBoxState
	 * @return
	 */
	Qt::CheckState GetForceCheckBoxState();

	/**
	 * @brief UpdateInputChannelCurrentValue
	 * @param in
	 */
	void UpdateInputChannelCurrentValue(ProcessImageIn *in);

	/**
	 * @brief UpdateOutputChannelCurrentValue
	 * @param out
	 */
	void UpdateOutputChannelCurrentValue(ProcessImageOut *out);

private slots:
	/**
	 * @brief on_check_stateChanged
	 * @param arg1
	 */
	void on_check_stateChanged(int arg1);

	/**
	 * @brief on_force_stateChanged
	 * @param arg1
	 */
	void on_force_stateChanged(int arg1);

private:
	Ui::ChannelFrame ui;
	// TODO change to ptr. TODO ptr in channelMap in api-lib
	Channel channel;

	//Q_DISABLE_COPY(Channel)
};

#endif // _UI_CHANNEL_H_


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
 * \brief The ChannelUi class
 */
class ChannelUi : public QWidget
{
	Q_OBJECT

public:
	/**
	 * \brief ChannelUi
	 * \param[in] channel
	 * \param[in] parent
	 */
	explicit ChannelUi(Channel channel, QWidget *parent = 0);

	~ChannelUi();

	/**
	 * \brief UpdateSelectCheckBox
	 *
	 * \param[in] forceState
	 */
	void UpdateSelectCheckBox(Qt::CheckState forceState);

	/**
	 * \brief GetSelectCheckBoxState
	 *
	 * \return the state of the check box.
	 */
	Qt::CheckState GetSelectCheckBoxState() const;

	/**
	 * \brief UpdateForceCheckBox
	 * \param forceState
	 */
	void UpdateForceCheckBox(Qt::CheckState forceState);

	/**
	 * \brief UpdateInputChannelCurrentValue
	 * \param in
	 */
	void UpdateInputChannelCurrentValue(ProcessImageIn *in);

	/**
	 * \brief UpdateOutputChannelCurrentValue
	 * \param out
	 */
	void UpdateOutputChannelCurrentValue(const ProcessImageOut *out);

private:
	Ui::ChannelFrame ui;
	// TODO change to ptr. TODO ptr in channelMap in api-lib
	Channel channel;

	/**
	 * \brief SetCurrentValue
	 * \param[in] setStr
	 */
	void SetCurrentValue(QString setStr);

	/**
	 * \brief GetForceValue
	 * \return
	 */
	const QString GetForceValue() const;

	/**
	 * \brief GetForceCheckBoxState
	 * \return
	 */
	Qt::CheckState GetForceCheckBoxState() const;
};

#endif // _UI_CHANNEL_H_


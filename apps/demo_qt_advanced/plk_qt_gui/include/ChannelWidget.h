/**
********************************************************************************
\file   ChannelWidget.h

\brief  Provides an user-interface instance of an individual channel in
		the ProcessImage.

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

#ifndef _CHANNEL_WIDGET_H_
#define _CHANNEL_WIDGET_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "ui_ChannelWidget.h"
#include "user/processimage/Channel.h"

class LineEditWidget;
class ProcessImageIn;
class ProcessImageOut;

/**
 * \brief The ChannelWidget class constructs the single frame for the each channels
 *  present in the processimage
 */
class ChannelWidget : public QWidget
{
	Q_OBJECT

public:
	/**
	 * \brief Constructs the ChannelWidget instance and holds the properties of
	 * the channel in it.
	 *
	 * \param[in] channel  The instance for which the UI frame is generated.
	 * \param[in] parent
	 */
	explicit ChannelWidget(const Channel &channel, QWidget *parent = 0);

	~ChannelWidget();

	/**
	 * \brief Sets the state of the 'select check box'.
	 *
	 * \param[in] state The state of the check box.
	 */
	void SetSelectCheckBox(Qt::CheckState state);

	/**
	 * \return the state of the select check box.
	 */
	Qt::CheckState GetSelectCheckBoxState() const;

	/**
	 * \brief Updates the Input Channel's Current Value.
	 *
	 * Updates the current value and if any value is forced, it updates the
	 * force value to the input processimage.
	 *
	 * \param[in,out] in Input processImage instance.
	 */
	void UpdateInputChannelCurrentValue(ProcessImageIn *in);

	/**
	 * \brief Updates the Output Channel's Current Value.
	 *
	 * \param[in] out Output processImage instance.
	 */
	void UpdateOutputChannelCurrentValue(const ProcessImageOut *out);

private slots:
	/**
	 * \brief Updates the value to the processimage whenever the
	 * return key pressed inside the LineEditWidget.
	 */
	void ValueReturnPressed();

	/**
	 * \brief Locks the current value from updating the processimage.
	 */
	void LockCurrentValue();

	/**
	 * \brief Unlocks the current value from updating the processimage.
	 */
	void UnlockCurrentValue();

private:
	Ui::ChannelWidget ui;    ///< Channel ui instance.
	LineEditWidget *value;   ///< Current value textbox.
	// TODO change to ptr. TODO ptr in channelMap in api-lib
	const Channel channel;   ///< ProcessImage channel properties.

	bool lockValueTexbox;    ///< Property to lock the textbox while forcing the value.
	QString valueBeforeLock;

	ProcessImageIn *input; ///< Input ProcessImage properties.

	/**
	 * \brief Sets the current value text box with the given string.
	 *
	 * \param[in] setStr The string to be updated in the textbox.
	 */
	void SetCurrentValue(QString setStr);

	/**
	 * \brief Sets the input mask for the current value textbox
	 * based on the channel's properties.
	 */
	void SetInputMask();
};

#endif // _CHANNEL_WIDGET_H_

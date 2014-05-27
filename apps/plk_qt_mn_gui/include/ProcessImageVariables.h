/**
********************************************************************************
\file   ProcessImageVariables.h

\brief  Describes the variables view of the input and output processimage data
		from the oplk stack.

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

#ifndef _PROCESSIMAGE_VARIABLES_H_
#define _PROCESSIMAGE_VARIABLES_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "ui_ProcessImageVariables.h"
#include "ChannelWidget.h"
#include <QtCore/QList>

#include "user/processimage/ProcessImageIn.h"
#include "user/processimage/ProcessImageOut.h"

/**
 * \brief The ProcessImageVariables class inherits the QWidget and describes the
 * logic to update the input and output processimage data from the OPlk stack.
 */
class ProcessImageVariables : public QWidget
{
	Q_OBJECT

public:
	/**
	 * \brief Constructs the ProcessImageVariables ui instance and prepares the
	 * variables row based on the number of channels present in the
	 * ProcessImageIn and ProcessImageOut.
	 *
	 * \param[in] parent
	 */
	explicit ProcessImageVariables(QWidget *parent = 0);

	~ProcessImageVariables();

public slots:

	/**
	 * \brief Sets the input and output processimage instance thereby
	 * prepares the input and output variables view.
	 *
	 * \param inPi Input processimage instance.
	 * \param outPi Output processimage instance.
	 */
	void SetProcessImage(ProcessImageIn *inPi, const ProcessImageOut *outPi);

	/**
	 * \brief Updates the processimage input channels value from and/or to the stack.
	 */
	void UpdateFromInputValues();

	/**
	 * \brief Update the processimage output channels value from the stack.
	 */
	void UpdateFromOutputValues();

	/**
	 * \brief Removes the channel widgets.
	 */
	void ResetView();

private slots:
	/**
	 * \brief Updates the state of the all the channels select check box
	 * for the input processimage table.
	 *
	 * \param checkedState The state of the check box.
	 */
	void on_inputCheckAll_stateChanged(int checkedState);

	/**
	 * \brief Updates the state of the all the channels select check box
	 * for the output processimage table.
	 *
	 * \param checkedState The state of the check box.
	 */
	void on_outputCheckAll_stateChanged(int checkedState);

	/**
	 * \brief Hides all the channels selected in the input processimage.
	 */
	void on_inputHideCheckedBtn_clicked();

	/**
	 * \brief Shows all the channels present in the input processimage.
	 */
	void on_inputShowAllBtn_clicked();

	/**
	 * \brief Hides all the channels selected in the output processimage.
	 */
	void on_outHideCheckedBtn_clicked();

	/**
	 * \brief Shows all the channels present in the output processimage.
	 */
	void on_outShowAllBtn_clicked();

private:
	Ui::ProcessImageVariables ui;      ///< processimage variables ui instance.

	QList<ChannelWidget*> inputChannels;   ///< Input channel ui instance list.
	QList<ChannelWidget*> outputChannels;  ///< Output channel ui instance list.

	ProcessImageIn *inPi;              ///< Input processimage instance.
	const ProcessImageOut *outPi;      ///< Output processimage instance.

	const int processImageInputSlotIndex;
	const int processImageOutputSlotIndex;

	/**
	 * \brief Generates the rows of ui channels by parsing the input
	 * processimage instance.
	 */
	void PrepareInputRows();

	/**
	 * \brief Generates the rows of ui channels by parsing the output
	 * processimage instance.
	 */
	void PrepareOutputRows();
};

#endif // _PROCESSIMAGE_VARIABLES_H_

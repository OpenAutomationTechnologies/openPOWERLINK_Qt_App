/**
********************************************************************************
\file   ProcessImageVariables.h

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

#ifndef _PROCESSIMAGEVARIABLES_H_
#define _PROCESSIMAGEVARIABLES_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "ui_ProcessImageVariables.h"
#include "ChannelUi.h"
#include <QList>

#include "user/processimage/ProcessImageIn.h"
#include "user/processimage/ProcessImageOut.h"

class ProcessImageVariables : public QFrame
{
	Q_OBJECT

public:
	explicit ProcessImageVariables(ProcessImageIn &in, ProcessImageOut &out, QWidget *parent = 0);

	Qt::CheckState GetInputSelectAllCheckBoxState() const;

	Qt::CheckState GetOutputSelectAllCheckBoxState() const;

	~ProcessImageVariables();

public slots:
	void UpdateInputs();

	void UpdateOutputs();

private slots:
	void on_inputCheckAll_stateChanged(int arg1);

	void on_outputCheckAll_stateChanged(int arg1);

	void on_inputForceAll_stateChanged(int arg1);

	void on_inputHideCheckedBtn_clicked();

	void on_inputShowAllBtn_clicked();

	void on_outHideCheckedBtn_clicked();

	void on_outShowAllBtn_clicked();

private:
	Ui::ProcessImageVariables ui;
	QList<ChannelUi*> inputChannels;
	QList<ChannelUi*> outputChannels;

	ProcessImageIn &inPi;
	ProcessImageOut &outPi;

	void PrepareInputRows();
	void PrepareOutputRows();
};

#endif // _PROCESSIMAGEVARIABLES_H_

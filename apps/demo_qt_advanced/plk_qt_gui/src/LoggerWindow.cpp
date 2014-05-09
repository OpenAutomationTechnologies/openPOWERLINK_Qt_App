/**
********************************************************************************
\file   LoggerWindow.cpp

\brief  Implements the Logging mechanism and the actions involved in it.

\todo
		- New UI for the logger window which gives user the option to select
			receiving the logs

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
#include "LoggerWindow.h"

#include "api/OplkQtApi.h"

/*******************************************************************************
* Public functions
*******************************************************************************/

LoggerWindow::LoggerWindow(QWidget *parent) :
	QDockWidget(parent),
	copyAvailable(false)
{
	this->ui.setupUi(this);
	bool ret = OplkQtApi::RegisterEventLogger(*this, this->metaObject()->method(
				this->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(
				"HandleStackLog(const QString&)").constData())));
	Q_ASSERT(ret != false);
}

void LoggerWindow::HandleStackLog(const QString& str)
{
	this->ui.log->appendPlainText(str);
}

void LoggerWindow::on_copyStack_clicked()
{
	if (!this->copyAvailable)
		this->ui.log->selectAll();
	this->ui.log->copy();
}

void LoggerWindow::on_log_copyAvailable(bool txtSelected)
{
	this->copyAvailable = txtSelected;
}

void LoggerWindow::on_clearStack_clicked()
{
	this->ui.log->clear();
	this->ui.log->setPlainText("");
}

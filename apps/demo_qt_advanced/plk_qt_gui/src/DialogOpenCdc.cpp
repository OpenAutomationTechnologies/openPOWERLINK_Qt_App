/**
********************************************************************************
\file   DialogOpenCdc.cpp

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

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "DialogOpenCdc.h"
#include <QFileDialog>
#include <QFile>

/*******************************************************************************
* Public functions
*******************************************************************************/

DialogOpenCdc::DialogOpenCdc(QWidget *parent) :
	QDialog(parent)
{
	this->ui.setupUi(this);
}

/*******************************************************************************
* Private functions
*******************************************************************************/

void DialogOpenCdc::on_browseCDC_clicked()
{
	// for 3rd parameter use default path or last used path using QSettings
	this->cdcPath = QFileDialog::getOpenFileName(this, "Choose CDC...",
					 QString(), tr("Concise Device Configuration File(*.cdc *.CDC)"));
	qDebug(this->cdcPath.toStdString().c_str());
	this->ui.cdcPath->setText(this->cdcPath);
}

void DialogOpenCdc::on_openCdcDialog_accepted()
{
	// CDC file will be exist because it is selected via ui. If not the user pasted the value
	// Also check for is readable and symlink QFileInfo
	if(QFile::exists(this->cdcPath))
	{

	}
	this->on_browseCDC_clicked();
	// Strip /*.cdc and append xap.xml and check if file exists. and return error here.
}

void DialogOpenCdc::on_openCdcDialog_rejected()
{

}

void DialogOpenCdc::on_cdcPath_textEdited(const QString &arg1)
{
	this->on_browseCDC_clicked();
}

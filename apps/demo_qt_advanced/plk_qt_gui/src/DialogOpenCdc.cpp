/**
********************************************************************************
\file   DialogOpenCdc.cpp

\brief  Implements the Open CDC dialog and the actions involved in it.

\todo
		- Implement getting the file path for the xap.xml

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
	QDialog(parent),
	cdcFile(""),
	xapFile("")
{
	this->ui.setupUi(this);
}

/*******************************************************************************
* Private functions
*******************************************************************************/

void DialogOpenCdc::on_browseCDC_clicked()
{
	QString cdcPath = QFileDialog::getExistingDirectory(this, tr("Choose the path for the CDC and xap.xml"),
						this->ui.cdcPath->text(),
						QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	qDebug(cdcPath.toStdString().c_str());
	this->cdcFile = cdcPath.toStdString();
	cdcFile.append("/mnobd.cdc");

	this->ui.cdcPath->setText(QString::fromStdString(this->cdcFile));

	this->xapFile = cdcPath.toStdString();
	xapFile.append("/xap.xml");

	this->ui.xapPath->setText(QString::fromStdString(this->xapFile));
}

const char* DialogOpenCdc::GetCdcFileName() const
{
	if (!this->cdcFile.empty())
		return this->cdcFile.c_str();
	else
		return NULL;
}

const char* DialogOpenCdc::GetXapFileName() const
{
	if (!this->xapFile.empty())
		return this->xapFile.c_str();
	else
		return NULL;
}

void DialogOpenCdc::on_okButton_clicked()
{
	if (QFile::exists(QString::fromStdString(this->cdcFile))
		&& QFile::exists(QString::fromStdString(this->xapFile)))
	{
		this->accept();
	}
	else
	{
		this->ui.message->setText("<font color='red'>CDC and/or xap is not found in the path specified</font>");
	}
}

void DialogOpenCdc::on_cdcPath_editingFinished()
{
	this->cdcFile = this->ui.cdcPath->text().toStdString();
}

void DialogOpenCdc::on_xapPath_editingFinished()
{
	this->xapFile = this->ui.xapPath->text().toStdString();
}

void DialogOpenCdc::on_cancelButton_clicked()
{
	this->reject();
}

/**
********************************************************************************
\file   DialogOpenCdc.cpp

\brief  Implements the Open CDC dialog and the actions involved in it.

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
	this->ui.okButton->setEnabled(false);
}

/*******************************************************************************
* Private functions
*******************************************************************************/

void DialogOpenCdc::on_browseCDC_clicked()
{
	this->ui.message->clear();

	QString cdcPath = QFileDialog::getOpenFileName(this,
								tr("Open File"),
								this->ui.cdcPath->text(),
								tr("Concise device configuration (*.cdc *.CDC);;All files (*)"));

	QFileInfo cdcfile(cdcPath);

	if (cdcfile.exists())
	{
		this->cdcFile = cdcPath.toStdString();

		this->xapFile = cdcfile.path().toStdString();
		this->xapFile.append("/xap.xml");
		qDebug(cdcPath.toStdString().c_str());
	}

	this->ui.cdcPath->setText(QString::fromStdString(this->cdcFile));
	this->ui.xapPath->setText(QString::fromStdString(this->xapFile));
}

void DialogOpenCdc::on_browseXap_clicked()
{
	this->ui.message->clear();

	QString xapPath = QFileDialog::getOpenFileName(this,
								tr("Open File"),
								this->ui.xapPath->text(),
								tr("XML application process variables (*.xml *.XML);;All files (*)"));

	QFileInfo xapFile(xapPath);

	if (xapFile.exists())
	{
		this->xapFile = xapPath.toStdString();

		this->cdcFile = xapFile.path().toStdString();
		this->cdcFile.append("/mnobd.cdc");
		qDebug(xapPath.toStdString().c_str());
	}

	this->ui.cdcPath->setText(QString::fromStdString(this->cdcFile));
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
	bool error = false;
	this->ui.message->clear();
	if (!QFile::exists(QString::fromStdString(this->cdcFile)))
	{
		error = true;
		this->ui.message->setText("<font color='red'>CDC not found.</font>");
	}

	if (!QFile::exists(QString::fromStdString(this->xapFile)))
	{
		error = true;
		this->ui.message->setText(this->ui.message->text() + "<font color='red'>Xap not found.</font>");
	}

	if (!error)
		this->accept();
}

void DialogOpenCdc::on_cancelButton_clicked()
{
	this->reject();
}

void DialogOpenCdc::on_cdcPath_textChanged(const QString &arg1)
{
	bool cdcExists = QFile::exists(arg1) && QFileInfo(arg1).isFile();
	bool xapExists = QFile::exists(this->ui.xapPath->text())
					 && QFileInfo(this->ui.xapPath->text()).isFile();
	this->ui.okButton->setEnabled(cdcExists && xapExists);

	if (cdcExists)
		this->cdcFile = arg1.toStdString();

	//TODO Change to CdcMessage and xapMessage
	this->ui.message->clear();
	if (!cdcExists)
	{
		this->ui.message->setText("<font color='red'>CDC not found.</font>");
	}
	else
	{
		this->ui.message->setText("<font color='green'>CDC found.</font>");
	}
}

void DialogOpenCdc::on_xapPath_textChanged(const QString &arg1)
{
	bool cdcExists = QFile::exists(this->ui.cdcPath->text())
					 && QFileInfo(this->ui.cdcPath->text()).isFile();
	bool xapExists = QFile::exists(arg1) && QFileInfo(arg1).isFile();
	this->ui.okButton->setEnabled(cdcExists && xapExists);

	if (xapExists)
		this->xapFile = arg1.toStdString();

	this->ui.message->clear();
	if (!xapExists)
	{
		this->ui.message->setText("<font color='red'>Xap not found.</font>");
	}
	else
	{
		this->ui.message->setText("<font color='green'>Xap found.</font>");
	}
}

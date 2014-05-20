/**
********************************************************************************
\file   ImportConfigurationDialog.cpp

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
#include "ImportConfigurationDialog.h"
#include <QFileDialog>
#include <QFile>

/*******************************************************************************
* Public functions
*******************************************************************************/

ImportConfigurationDialog::ImportConfigurationDialog(QWidget *parent) :
	QDialog(parent),
	cdcFile(""),
	xapFile("")
{
	this->ui.setupUi(this);
}

const QString& ImportConfigurationDialog::GetCdcFileName() const
{
	return this->cdcFile;
}

const QString& ImportConfigurationDialog::GetXapFileName() const
{
	return this->xapFile;
}

/*******************************************************************************
* Private functions
*******************************************************************************/

void ImportConfigurationDialog::on_browseCDC_clicked()
{
	this->ui.xapError->clear();
	QString cdc = QFileDialog::getOpenFileName(this,
								tr("Open File"),
								this->ui.cdcPath->text(),
								tr("Concise device configuration (*.cdc *.CDC);;All files (*)"));
	if (!cdc.isEmpty())
	{
		this->ui.cdcPath->setText(cdc);
		// Try to set the xap path from the same location of CDC
		// Only if the path is empty
		if (this->ui.xapPath->text().isEmpty())
		{
			QFileInfo cdcfile(cdc);
			QString xap = cdcfile.path();
			xap.append("/xap.xml");
			if (!QFileInfo::exists(xap))
				this->SetErrorMessage(XAP_XML, false);
			this->ui.xapPath->setText(xap);
		}
	}
}

void ImportConfigurationDialog::on_browseXap_clicked()
{
	this->ui.xapError->clear();

	QString xap = QFileDialog::getOpenFileName(this,
								tr("Open File"),
								this->ui.xapPath->text(),
								tr("XML application process variables (*.xml *.XML);;All files (*)"));
	if (!xap.isEmpty())
	{
		this->ui.xapPath->setText(xap);

		// Try to set the CDC path from the same location of Xap
		// Only if the path is empty
		if (this->ui.cdcPath->text().isEmpty())
		{
			QFileInfo xapFile(xap);
			QString cdc = xapFile.path();
			cdc.append("/mnobd.cdc");
			if (!QFileInfo::exists(cdc))
				this->SetErrorMessage(CDC, false);

			this->ui.cdcPath->setText(cdc);
		}
	}
}

void ImportConfigurationDialog::on_okButton_clicked()
{
	bool cdcExists = QFileInfo::exists(this->ui.cdcPath->text());
	bool xapExists = QFileInfo::exists(this->ui.xapPath->text());

	if (!cdcExists)
		this->SetErrorMessage(CDC, cdcExists);

	if (!xapExists)
		this->SetErrorMessage(XAP_XML, xapExists);

	if (cdcExists && xapExists)
	{
		this->cdcFile = this->ui.cdcPath->text();
		this->xapFile = this->ui.xapPath->text();

		emit this->SignalCdcChanged(this->cdcFile);
		emit this->SignalXapChanged(this->xapFile);
		this->accept();
	}
}

void ImportConfigurationDialog::on_cancelButton_clicked()
{
	this->ui.cdcPath->setText(this->cdcFile);
	this->ui.xapPath->setText(this->xapFile);
	this->reject();
}

void ImportConfigurationDialog::on_cdcPath_textChanged(const QString &cdc)
{
	bool cdcExists = QFileInfo::exists(cdc) && QFileInfo(cdc).isFile();
	bool xapExists = QFileInfo::exists(this->ui.xapPath->text())
					 && QFileInfo(this->ui.xapPath->text()).isFile();

	this->ui.okButton->setEnabled(cdcExists && xapExists);

	this->SetErrorMessage(CDC, cdcExists);
}

void ImportConfigurationDialog::on_xapPath_textChanged(const QString &xap)
{
	bool cdcExists = QFileInfo::exists(this->ui.cdcPath->text())
					 && QFileInfo(this->ui.cdcPath->text()).isFile();
	bool xapExists = QFileInfo::exists(xap) && QFileInfo(xap).isFile();

	this->ui.okButton->setEnabled(cdcExists && xapExists);

	this->SetErrorMessage(XAP_XML, xapExists);
}

void ImportConfigurationDialog::SetErrorMessage(Configuration cfg, bool exists)
{
	if (cfg == CDC)
	{
		this->ui.cdcError->clear();
		if (exists)
			this->ui.cdcError->setText("<font color='green'>CDC found.</font>");
		else
			this->ui.cdcError->setText("<font color='red'>CDC not found.</font>");
	}
	else
	{
		this->ui.xapError->clear();
		if (exists)
			this->ui.xapError->setText("<font color='green'>Xap found.</font>");
		else
			this->ui.xapError->setText("<font color='red'>Xap not found.</font>");
	}
}

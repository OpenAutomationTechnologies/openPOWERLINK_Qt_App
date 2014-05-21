/**
********************************************************************************
\file   ImportConfigurationDialog.h

\brief  Refer ImportConfigurationDialog class

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

#ifndef _IMPORT_CONFIGURATION_DIALOG_H_
#define _IMPORT_CONFIGURATION_DIALOG_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/

#include "ui_ImportConfigurationDialog.h"
#include <QtCore/QString>

/**
 * \brief The ImportConfigurationDialog class inherits the QDialog to read the CDC and
 * xap.xml path from the user.
 */
class ImportConfigurationDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ImportConfigurationDialog(QWidget *parent = 0);

	/**
	 * \return The CDC's full path with name mnobd.cdc
	 */
	const QString& GetCdcFileName() const;

	/**
	 * \return The xap xml's full path with name xap.xml
	 */
	const QString& GetXapFileName() const;

private slots:
	/**
	 * \brief Allows the user to select the path of the CDC.
	 */
	void on_browseCDC_clicked();

	/**
	 * \brief Allows the user to select the path of the Xap.xml.
	 */
	void on_browseXap_clicked();

	/**
	 * \brief Validates the availability of the CDC and xap.xml files.
	 */
	void on_okButton_clicked();

	/**
	 * \brief Closes the CDC dialog.
	 */
	void on_cancelButton_clicked();

	/**
	 * \brief Validates the CDC path and updates error if any.
	 * \param[in] cdc Changed CDC file.
	 */
	void on_cdcPath_textChanged(const QString &cdc);

	/**
	 * \brief Validates the XAP path and updates error if any.
	 * \param[in] xap Changed Xap file.
	 */
	void on_xapPath_textChanged(const QString &xap);

signals:
	void SignalCdcChanged(QString& cdcFile);
	void SignalXapChanged(QString& xapFile);

private:
	Ui::ImportConfigurationDialog ui;

	QString cdcFile;
	QString xapFile;

	enum Configuration
	{
		CDC,
		XAP_XML
	};

	/**
	 * \brief Sets the error message with color based on the given exists status.
	 * \param[in] cfg    Specifies the configuration file type.
	 * \param[in] exists Specifies the existence status.
	 */
	void SetErrorMessage(Configuration cfg, bool exists);
};

#endif // _IMPORT_CONFIGURATION_DIALOG_H_

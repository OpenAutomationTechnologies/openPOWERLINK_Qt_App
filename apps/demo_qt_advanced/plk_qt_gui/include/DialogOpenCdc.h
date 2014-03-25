/**
********************************************************************************
\file   DialogOpenCdc.h

\brief  Refer DialogOpenCdc class

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

#ifndef _UI_DIALOG_OPEN_CDC_H_
#define _UI_DIALOG_OPEN_CDC_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/

#include "ui_DialogOpenCdc.h"

/**
 * \brief The DialogOpenCdc class inherits the QDialog to read the CDC and
 * xap.xml path from the user.
 */
class DialogOpenCdc : public QDialog
{
	Q_OBJECT

public:
	explicit DialogOpenCdc(QWidget *parent = 0);

private slots:
	/**
	 * \brief Allows the user to select the path of the CDC
	 */
	void on_browseCDC_clicked();

	/**
	 * \brief Validates the availability of the CDC and xap.xml files
	 */
	void on_openCdcDialog_accepted();

	/**
	 * \brief on_openCdcDialog_rejected
	 */
	void on_openCdcDialog_rejected();

	/**
	 * \brief on_cdcPath_textEdited
	 * \param[in] arg1 unused
	 */
	void on_cdcPath_textEdited(const QString &arg1);

private:
	Ui::DialogOpenCdc ui;
	QString cdcPath;
};

#endif // _UI_DIALOG_OPEN_CDC_H_

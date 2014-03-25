/**
********************************************************************************
\file   SelectNwInterfaceDialog.h

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

#ifndef _SELECTNWINTERFACEDIALOG_H_
#define _SELECTNWINTERFACEDIALOG_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "ui_SelectNwInterfaceDialog.h"

/**
 * \brief The SelectNwInterfaceDialog class
 */
class SelectNwInterfaceDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * \brief SelectNwInterfaceDialog
	 * \param parent
	 */
	explicit SelectNwInterfaceDialog(QWidget *parent = 0);

	/**
	 * \brief GetDevName
	 * \return
	 */
	QString GetDevName(void) const;

	/**
	 * \brief GetDevDescription
	 * \return
	 */
	QString GetDevDescription(void) const;

	/**
	 * \brief FillList
	 * \return
	 */
	int FillList(void);

private slots:
	/**
	 * \brief on_listWidget_itemDoubleClicked
	 * \param item
	 */
	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

	/**
	 * \brief on_buttonBox_accepted
	 */
	void on_buttonBox_accepted();

	/**
	 * \brief on_buttonBox_rejected
	 */
	void on_buttonBox_rejected();

private:
	Ui::SelectNwInterfaceDialog ui; /// Select Network interface ui instance
	QString devName;              /// network device name
	QString devDescription;         /// network device description
};

#endif // _SELECTNWINTERFACEDIALOG_H_

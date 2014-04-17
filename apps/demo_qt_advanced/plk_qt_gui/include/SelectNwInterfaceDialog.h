/**
********************************************************************************
\file   SelectNwInterfaceDialog.h

\brief  Refer SelectNwInterfaceDialog class

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

#ifndef _SELECT_NW_INTERFACE_DIALOG_H_
#define _SELECT_NW_INTERFACE_DIALOG_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "ui_SelectNwInterfaceDialog.h"

/**
 * \brief The network interface select dialog class
 *
 * Describes the logic to query the network interface available in any
 * system using the PCAP interface.
 */
class SelectNwInterfaceDialog : public QDialog
{
	Q_OBJECT

public:

	explicit SelectNwInterfaceDialog(QWidget *parent = 0);

	/**
	 * \return The selected network device name
	 */
	QString GetDevName(void) const;

	/**
	 * \return The selected network device's description
	 */
	QString GetDevDescription(void) const;

	//TODO bool or return number of items in list.
	/**
	 * \brief Fills the listwidget with the list of available network interface
	 * descriptions.
	 * \retval -1 if no list found.(fail)
	 * \retval 0  if list has valid entries.(success)
	 */
	int FillList(void);

private slots:
	/**
	 * \brief Marks the item for selection and exits the dialog.
	 * \param item The selected item.
	 */
	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

	/**
	 * \brief Closes the dialog and and saves the selected item.
	 */
	void on_ok_clicked();

	/**
	 * \brief Cancels the item selection process.
	 */
	void on_cancel_clicked();

private:
	Ui::SelectNwInterfaceDialog ui; /// Select Network interface ui instance
	QString devName;                /// network device name
	QString devDescription;         /// network device description
};

#endif // _SELECT_NW_INTERFACE_DIALOG_H_

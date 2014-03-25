/**
********************************************************************************
\file   SelectNwInterfaceDialog.cpp

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
#include "SelectNwInterfaceDialog.h"
#include <pcap.h>


/*******************************************************************************
* Public functions
*******************************************************************************/
SelectNwInterfaceDialog::SelectNwInterfaceDialog(QWidget *parent) :
	QDialog(parent),
	devName("")
{
	this->ui.setupUi(this);
}

int SelectNwInterfaceDialog::FillList(void)
{
	// Clear list before filling
	this->ui.listWidget->clear();

	char        sErr_Msg[PCAP_ERRBUF_SIZE];
	pcap_if_t * alldevs;
	int         numIntf = 0;

	/* Retrieve the device list on the local machine */
	if (pcap_findalldevs(&alldevs, sErr_Msg) == -1)
	{
		return -1;
	}

	/* Add the list to the listbox */
	for (pcap_if_t *seldev = alldevs; seldev != NULL; seldev = seldev->next)
	{
		QString devDesc;
		if (seldev->description)
		{
			devDesc = seldev->description;
		}
		else
		{
			devDesc = seldev->name;
		}
		this->ui.listWidget->addItem(devDesc);

		QVariant data(QString(seldev->name));
		QListWidgetItem *newItem = this->ui.listWidget->item(numIntf);
		newItem->setData(Qt::UserRole, data);
		newItem->setText(devDesc);

		++numIntf;
	}
	pcap_freealldevs(alldevs);

	if (numIntf > 0)
		return 0;
	else
		return -1;
}

QString SelectNwInterfaceDialog::GetDevName(void) const
{
	return this->devName;
}

QString SelectNwInterfaceDialog::GetDevDescription(void) const
{
	return this->devDescription;
}

/*******************************************************************************
* Private functions
*******************************************************************************/
void SelectNwInterfaceDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
	this->devName = item->data(Qt::UserRole).toString();
	this->devDescription = item->text();
	this->ui.buttonBox->accepted();
}

void SelectNwInterfaceDialog::on_buttonBox_accepted()
{
	// Only Single selection so taking 0;
	//FIXME error while pressing ok without selecting any item. SelectedItems returns NULL
	QListWidgetItem *item = this->ui.listWidget->selectedItems().at(0);
	if (item)
	{
		this->devName = item->data(Qt::UserRole).toString();
		this->devDescription = item->text();
	}
}

void SelectNwInterfaceDialog::on_buttonBox_rejected()
{
	this->devName = "";
	this->devDescription = "";
}

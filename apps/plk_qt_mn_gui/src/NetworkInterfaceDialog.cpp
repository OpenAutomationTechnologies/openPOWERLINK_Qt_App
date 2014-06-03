/**
********************************************************************************
\file   NetworkInterfaceDialog.cpp

\brief  Implements the network selection dialog using the PCAP interface.

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
#include "NetworkInterfaceDialog.h"

#ifdef CONFIG_USE_PCAP
	#if (TARGET_SYSTEM == _WIN32_)
		#define _WINSOCKAPI_
	#endif  // (TARGET_SYSTEM == _WIN32_)
	#include <pcap.h>
#else // CONFIG_USE_PCAP
	#include <oplk/powerlink-module.h>
#endif // CONFIG_USE_PCAP

/*******************************************************************************
* Public functions
*******************************************************************************/
NetworkInterfaceDialog::NetworkInterfaceDialog(QWidget *parent) :
	QDialog(parent),
	devName(""),
	devDescription("")
{
	this->ui.setupUi(this);
}

int NetworkInterfaceDialog::FillList(void)
{
	// Clear list before filling
	this->ui.listWidget->clear();
	this->ui.ok->setEnabled(false);

	int numberOfInterfaces = 0;

#ifdef CONFIG_USE_PCAP
	char        sErr_Msg[PCAP_ERRBUF_SIZE];
	pcap_if_t *alldevs = NULL;

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
		QListWidgetItem *newItem = this->ui.listWidget->item(numberOfInterfaces);
		newItem->setData(Qt::UserRole, data);
		newItem->setText(devDesc);

		++numberOfInterfaces;
	}
	pcap_freealldevs(alldevs);

#else // CONFIG_USE_PCAP
	QString devDesc(PLK_DEV_NAME);

	this->ui.listWidget->addItem(devDesc);

	QVariant data(devDesc);
	QListWidgetItem *newItem = this->ui.listWidget->item(numberOfInterfaces);
	newItem->setData(Qt::UserRole, data);
	newItem->setText(devDesc);

	++numberOfInterfaces;
#endif // CONFIG_USE_PCAP

	if (numberOfInterfaces > 0)
		return 0;
	else
		return -1;
}

QString NetworkInterfaceDialog::GetDevName(void) const
{
	return this->devName;
}

QString NetworkInterfaceDialog::GetDevDescription(void) const
{
	return this->devDescription;
}

/*******************************************************************************
* Private Slots
*******************************************************************************/
void NetworkInterfaceDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
	this->devName = item->data(Qt::UserRole).toString();
	this->SetDevDescritpion(item->text());
	this->accept();
}

void NetworkInterfaceDialog::on_ok_clicked()
{
	QList<QListWidgetItem*> list = this->ui.listWidget->selectedItems();
	if (list.count() > 0)
	{
		QListWidgetItem *item = list.at(0);
		if (item)
		{
			this->devName = item->data(Qt::UserRole).toString();
			this->SetDevDescritpion(item->text());
			this->accept();
		}
	}
}

void NetworkInterfaceDialog::on_cancel_clicked()
{
	this->reject();
}

void NetworkInterfaceDialog::on_listWidget_itemSelectionChanged()
{
	this->ui.ok->setEnabled(true);
}

/*******************************************************************************
* Private Functions
*******************************************************************************/
void NetworkInterfaceDialog::SetDevDescritpion(const QString& name)
{
	this->devDescription = name;
	emit SignalNetworkInterfaceChanged(this->devDescription);
}

#include "SelectNwInterfaceDialog.h"
#include <pcap.h>

SelectNwInterfaceDialog::SelectNwInterfaceDialog(QWidget *parent) :
	QDialog(parent),
	m_devName("")
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

		// QString devName(seldev->name);
		// QVariant data(devName);
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
	return this->m_devName;
}

QString SelectNwInterfaceDialog::GetDevDescription(void) const
{
	return this->devDescription;
}

void SelectNwInterfaceDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
	qDebug("double clicked");
	this->m_devName = item->data(Qt::UserRole).toString();
	//m_devName = item->data(Qt::StatusTipRole).toString();
	this->devDescription = item->text();
	this->ui.buttonBox->accepted();
}

void SelectNwInterfaceDialog::on_buttonBox_accepted()
{
	// Only Single selection so taking 0;
	QListWidgetItem *item = this->ui.listWidget->selectedItems().at(0);
	if (item)
	{
		this->m_devName = item->data(Qt::UserRole).toString();
		this->devDescription = item->text();
	}
}

void SelectNwInterfaceDialog::on_buttonBox_rejected()
{
	this->m_devName = "";
	this->devDescription = "";
}

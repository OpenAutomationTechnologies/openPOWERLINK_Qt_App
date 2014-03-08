#include "SelectNwInterfaceDialog.h"
#include <pcap.h>

SelectNwInterfaceDialog::SelectNwInterfaceDialog(QWidget *parent) :
	QDialog(parent),
	m_devName("")
{
	this->ui.setupUi(this);
}

int SelectNwInterfaceDialog::fillList(void)
{
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

		QString devName(seldev->name);
		QVariant data(devName);
		QListWidgetItem *newItem = ui.listWidget->item(numIntf);
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

QString SelectNwInterfaceDialog::getDevName(void)
{
	return m_devName;
}

void SelectNwInterfaceDialog::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	// set devName to the current Item
	// qDebug("Itemchanged");
	m_devName = current->data(Qt::UserRole).toString();
}

void SelectNwInterfaceDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
	qDebug("double clicked");
	m_devName = item->data(Qt::UserRole).toString();
	//m_devName = item->data(Qt::StatusTipRole).toString();
	this->ui.buttonBox->accepted();
}


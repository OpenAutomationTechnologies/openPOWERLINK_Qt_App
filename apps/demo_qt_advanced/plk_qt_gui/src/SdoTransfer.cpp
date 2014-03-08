#include "SdoTransfer.h"

SdoTransfer::SdoTransfer(QWidget *parent) :
	QFrame(parent)
{
	this->ui.setupUi(this);

/// Removes UDP for windows.(Requirement)
#ifdef _WIN32
	this->ui.sdoVia->removeItem(1);
#endif

}

void SdoTransfer::on_read_toggled(bool checked)
{
	//ui.sdoResultValue->setDisabled(checked);
	this->ui.sdoResultValue->setReadOnly(checked);
	qDebug("read: %d", checked);
}

void SdoTransfer::on_executeTransfer_clicked()
{

}

void SdoTransfer::on_nodeId_activated(int index)
{
	// this->ui.nodeId->addItems(NodeStatusDock::GetAvailableCnList());
}

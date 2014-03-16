#include "NmtCommandsDock.h"

#include <QMessageBox>
#include "api/OplkQtApi.h"
#include "oplk/debugstr.h"

NmtCommandsDock::NmtCommandsDock(QWidget *parent) :
	QDockWidget(parent)
{
	this->ui.setupUi(this);
}

void NmtCommandsDock::on_sendNmtBtn_clicked()
{
//	bool ok = false;
//	const int nodeid = this->ui.nodeId->text().toInt(&ok, 16);
//	if (!ok) {
//		//NodeId hex failed, handle error here
//		QMessageBox::critical(this, "Execute NmtCommand failed",
//							 this->ui.nodeId->text(),
//							 QMessageBox::Close);
//	}
	
	//Send NMT command is not working for MN. 
	// TODO in API library.
	const int nodeid = this->ui.nodeId->text().toInt(0, 16);
	qDebug("%d", nodeid);
	tOplkError oplkRet = OplkQtApi::ExecuteNmtCommand(nodeid, this->nmtCommand);
	if (oplkRet != kErrorOk)
	{
		QMessageBox::critical(this, "Execute NmtCommand failed",
							 QString("Execute NmtCommand falied with error: %1 ")
							  .arg(debugstr_getRetValStr(oplkRet)),
							 QMessageBox::Close);
		qDebug("Execute NmtCommand %x", oplkRet);
		return;
	}
}

void NmtCommandsDock::on_nmtCommand_currentIndexChanged(int index)
{
	/* It can be also done by parsing the string and getting the 0xXX
	 * code to pass for the NMT command enum
	 */

	switch (index)
	{
		case 0:
			this->nmtCommand = kNmtCmdInvalidService;
			break;
		case 1:
			this->nmtCommand = kNmtCmdStartNode;
			break;
		case 2:
			this->nmtCommand = kNmtCmdStopNode;
			break;
		case 3:
			this->nmtCommand = kNmtCmdEnterPreOperational2;
			break;
		case 4:
			this->nmtCommand = kNmtCmdEnableReadyToOperate;
			break;
		case 5:
			this->nmtCommand = kNmtCmdResetNode;
			break;
		case 6:
			this->nmtCommand = kNmtCmdResetCommunication;
			break;
		case 7:
			this->nmtCommand = kNmtCmdResetConfiguration;
			break;
		case 8:
			this->nmtCommand = kNmtCmdSwReset;
			break;
		default:
			this->nmtCommand = kNmtCmdInvalidService;
			break;
	}
}


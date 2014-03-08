#include "NmtCommandsDock.h"

NmtCommandsDock::NmtCommandsDock(QWidget *parent) :
	QDockWidget(parent)
{
	this->ui.setupUi(this);
}

void NmtCommandsDock::on_sendNmtBtn_clicked()
{
	// const int nodeid = this->ui.nodeId->text().toInt();
	// nmtCommand;
	// Execute NMT command (nodeid, nmtCommand);
}

void NmtCommandsDock::on_nmtCommand_currentIndexChanged(int index)
{
	/* It can be also done by parsing the string and getting the 0xXX
	 * code to pass for the NMT command enum
	 * /

	/* From UI list.
	"InvalidService(0xFF)"
	"StartNode(0x21)"
	"StopNode(0x22)"
	"EnterPreOperational2(0x23)"
	"EnableReadyToOperate(0x24)"
	"ResetNode(0x28)"
	"ResetCommunication(0x29)"
	"ResetConfiguration(0x2A)"
	"SwReset(0x2B)"
	*/
/*
	switch (index)
	{
		case 0:
			nmtCommand = kNmtCmdInvalidService;
			break;
		case 1:
			nmtCommand = kNmtCmdStartNode;
			break;
		case 2:
			nmtCommand = kNmtCmdStopNode;
			break;
		case 3:
			nmtCommand = kNmtCmdEnterPreOperational2;
			break;
		case 4:
			nmtCommand = kNmtCmdEnableReadyToOperate;
			break;
		case 5:
			nmtCommand = kNmtCmdResetNode;
			break;
		case 6:
			nmtCommand = kNmtCmdResetCommunication;
			break;
		case 7:
			nmtCommand = kNmtCmdResetConfiguration;
			break;
		case 8:
			nmtCommand = kNmtCmdSwReset;
			break;
		default:
			nmtCommand = kNmtCmdInvalidService;
			break;
	}
*/
}


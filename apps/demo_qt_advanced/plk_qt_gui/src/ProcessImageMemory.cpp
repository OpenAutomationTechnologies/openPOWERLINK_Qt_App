#include "ProcessImageMemory.h"

ProcessImageMemory::ProcessImageMemory(QWidget *parent) :
	QFrame(parent)
{
	ui.setupUi(this);
	ui.inputTable->resizeColumnsToContents();
	ui.outTable->resizeColumnsToContents();
}

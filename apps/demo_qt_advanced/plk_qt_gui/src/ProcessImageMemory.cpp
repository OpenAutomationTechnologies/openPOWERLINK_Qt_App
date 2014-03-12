#include "ProcessImageMemory.h"

const uint inSize = 17;
const uint outSize = 1;

ProcessImageMemory::ProcessImageMemory(QWidget *parent) :
	QFrame(parent)
{
	ui.setupUi(this);
	ui.inputTable->setRowCount((inSize/16) + 1);
	ui.outTable->setRowCount((outSize/16) + 1);

	ui.inputTable->verticalHeader()->setVisible(true);
	ui.outTable->verticalHeader()->setVisible(true);

	this->CreateVerticalHeaders();
	this->ResizeColumnsToContents();
}

void ProcessImageMemory::CreateVerticalHeaders()
{
	for (int i = 0; i < ((inSize / 16) + 1); ++i)
	{
		QTableWidgetItem *inputItem = new QTableWidgetItem();
		inputItem->setText((QString("%1").arg(i, 0, 16)).rightJustified(4, '0'));
		ui.inputTable->setVerticalHeaderItem(i, inputItem);
		// qDebug("%d",i);
	}

	for (uint i = 0; i < ((outSize / 16) + 1); ++i)
	{
		QTableWidgetItem *outputItem = new QTableWidgetItem();
		outputItem->setText((QString("%1").arg(i, 0, 16)).rightJustified(4, '0'));
		ui.outTable->setVerticalHeaderItem(i, outputItem);
		// qDebug("%d",i);
	}
}

void ProcessImageMemory::ResizeColumnsToContents()
{
	ui.inputTable->resizeColumnsToContents();
	ui.outTable->resizeColumnsToContents();
}

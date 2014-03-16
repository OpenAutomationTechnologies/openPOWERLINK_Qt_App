#include "ProcessImageMemory.h"

const uint inSize = 1490;
const uint outSize = 1;

ProcessImageMemory::ProcessImageMemory(QWidget *parent) :
	QFrame(parent)
{
	this->ui.setupUi(this);
	this->ui.inputTable->setRowCount((inSize / 16) + 1);
	this->ui.outTable->setRowCount((outSize / 16) + 1);

	this->ui.inputTable->verticalHeader()->setVisible(true);
	this->ui.outTable->verticalHeader()->setVisible(true);

	this->CreateVerticalHeaders();
	this->CreateCells();
	this->ResizeColumnsToContents();
}

ProcessImageMemory::~ProcessImageMemory()
{
	QTableWidgetItem *item = NULL;
	for (uint row = 0; row < this->ui.inputTable->rowCount(); ++row)
	{
		for (uint col = 0; col <this->ui.inputTable->columnCount(); ++col)
		{
			item = this->ui.inputTable->item(row, col);
			if (item != NULL)
			{
				delete item;
			}
		}
	}

	for (uint row = 0; row < this->ui.outTable->rowCount(); ++row)
	{
		for (uint col = 0; col <this->ui.outTable->columnCount(); ++col)
		{
			item = this->ui.outTable->item(row, col);
			if (item != NULL)
			{
				delete item;
			}
		}
	}

	for (uint row = 0; row < this->ui.inputTable->rowCount(); ++row)
	{
		item = this->ui.inputTable->verticalHeaderItem(row);
		if (item != NULL)
		{
			delete item;
		}
	}

	for (uint col = 0; col <this->ui.inputTable->columnCount(); ++col)
	{
		item = this->ui.inputTable->horizontalHeaderItem(col);
		if (item != NULL)
		{
			delete item;
		}
	}


	for (uint row = 0; row < this->ui.outTable->rowCount(); ++row)
	{
		item = this->ui.outTable->verticalHeaderItem(row);
		if (item != NULL)
		{
			delete item;
		}
	}

	for (uint col = 0; col <this->ui.outTable->columnCount(); ++col)
	{
		item = this->ui.outTable->horizontalHeaderItem(col);
		if (item != NULL)
		{
			delete item;
		}
	}
}

void ProcessImageMemory::CreateVerticalHeaders()
{
	QTableWidgetItem *item = NULL;
	for (uint i = 0; i < ((inSize / 16) + 1); ++i)
	{
		item = new QTableWidgetItem();
		item->setText((QString("%1").arg(i, 0, 16)).rightJustified(4, '0'));
		this->ui.inputTable->setVerticalHeaderItem(i, item);
	}

	for (uint i = 0; i < ((outSize / 16) + 1); ++i)
	{
		item = new QTableWidgetItem();
		item->setText((QString("%1").arg(i, 0, 16)).rightJustified(4, '0'));
		this->ui.outTable->setVerticalHeaderItem(i, item);
	}

//	this->ui.inputTable->setSortingEnabled(false);

//	item = this->ui.inputTable->item(0, 0);
//	item->setText("asfasf");
//	this->ui.inputTable->setSortingEnabled(this->ui.inputTable->isSortingEnabled());
}

void ProcessImageMemory::CreateCells()
{
	for (uint row = 0; row < this->ui.inputTable->rowCount(); ++row)
	{
		for (uint col = 0; col <this->ui.inputTable->columnCount(); ++col)
		{
			this->ui.inputTable->setItem(row, col, new QTableWidgetItem());
		}
	}

	for (uint row = 0; row < this->ui.outTable->rowCount(); ++row)
	{
		for (uint col = 0; col <this->ui.outTable->columnCount(); ++col)
		{
			this->ui.outTable->setItem(row, col, new QTableWidgetItem());
		}
	}
}

void ProcessImageMemory::ResizeColumnsToContents()
{
	this->ui.inputTable->resizeColumnsToContents();
	this->ui.outTable->resizeColumnsToContents();
}


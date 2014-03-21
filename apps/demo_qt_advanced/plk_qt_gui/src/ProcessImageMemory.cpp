#include "ProcessImageMemory.h"
#include "oplk/oplk.h"
#include <QDebug>
//const uint inSize = 1490;
//const uint outSize = 1;

ProcessImageMemory::ProcessImageMemory(ProcessImageIn &in, ProcessImageOut &out, QWidget *parent) :
	QFrame(parent),
	inPi(in),
	outPi(out)
{
	this->ui.setupUi(this);
	//this->ui.inputTable->setRowCount((in.GetSize() / 16) + 1);
	UINT inputRowCount = (in.GetSize() / this->ui.inputTable->columnCount()) + 1;
	UINT outputRowCount = (out.GetSize() / this->ui.outTable->columnCount()) + 1;
	this->ui.inputTable->setRowCount(inputRowCount);
	this->ui.outTable->setRowCount(outputRowCount);

	qDebug("Input: %d", in.GetSize());
	qDebug("Input: Row Count %d", this->ui.inputTable->rowCount());
	qDebug("Input: Col Count %d", this->ui.inputTable->columnCount());

	qDebug("Output: %d", out.GetSize());
	qDebug("Output: Row Count %d", this->ui.outTable->rowCount());
	qDebug("Output: Col Count %d", this->ui.outTable->columnCount());
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
		for (uint col = 0; col < this->ui.inputTable->columnCount(); ++col)
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
		for (uint col = 0; col < this->ui.outTable->columnCount(); ++col)
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

	for (uint col = 0; col < this->ui.inputTable->columnCount(); ++col)
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

	for (uint col = 0; col < this->ui.outTable->columnCount(); ++col)
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
	for (uint i = 0; i < ((this->inPi.GetSize() / this->ui.inputTable->columnCount()) + 1);
		 ++i)
	{
		item = new QTableWidgetItem();
		item->setText((QString("%1").arg(i * 16, 0, 16)).rightJustified(4, '0'));
		this->ui.inputTable->setVerticalHeaderItem(i, item);
	}

	for (uint i = 0; i < ((this->outPi.GetSize() / this->ui.outTable->columnCount()) + 1);
		 ++i)
	{
		item = new QTableWidgetItem();
		item->setFont(QFont("Courier", 9));
		item->setText((QString("%1").arg(i * 16, 0, 16)).rightJustified(4, '0'));
		this->ui.outTable->setVerticalHeaderItem(i, item);
	}
}

void ProcessImageMemory::CreateCells()
{
	QTableWidgetItem *item = NULL;
	for (uint row = 0; row < this->ui.inputTable->rowCount(); ++row)
	{
		for (uint col = 0; col <this->ui.inputTable->columnCount(); ++col)
		{
			item = new QTableWidgetItem();
			item->setFont(QFont("Courier", 9));
			this->ui.inputTable->setItem(row, col, item);
		}
	}

	for (uint row = 0; row < this->ui.outTable->rowCount(); ++row)
	{
		for (uint col = 0; col <this->ui.outTable->columnCount(); ++col)
		{
			item = new QTableWidgetItem();
			item->setFont(QFont("Courier", 9));
			this->ui.outTable->setItem(row, col, item);
		}
	}
}

void ProcessImageMemory::ResizeColumnsToContents()
{
	this->ui.inputTable->resizeColumnsToContents();
	this->ui.outTable->resizeColumnsToContents();
}

void ProcessImageMemory::UpdateInputValue()
{
	try
	{
		std::vector<BYTE> value = this->inPi.GetRawData((this->inPi.GetSize() * 8), 0);
		UINT row = 0;
		UINT col = 0;
		QTableWidgetItem *cell = NULL;
		//qDebug("Input size %d", value.size());
		for (std::vector<BYTE>::const_iterator it = value.begin();
				it != value.end(); ++it)
		{
			// string = QString("%1").arg(*it, 0, 16).rightJustified(2, '0');
			// qDebug("row:%d col:%d  val: %s", row, col, string.toUtf8().constData());
			bool isSortenabled = this->ui.inputTable->isSortingEnabled();
			this->ui.inputTable->setSortingEnabled(false);

			cell = this->ui.inputTable->item(row, col);
			if (cell)
				cell->setText(QString("%1").arg(*it, 0, 16).rightJustified(2, '0'));

			this->ui.inputTable->setSortingEnabled(isSortenabled);
			if ((col + 1) == this->ui.inputTable->columnCount())
			{
				++row;
				col = 0;
			}
			else
			{
				++col;
			}
		}
	}
	catch(const std::exception& ex)
	{
		qDebug("An Exception has occured: %s", ex.what());
	}
}

void ProcessImageMemory::UpdateOutputValue()
{
	try
	{
		std::vector<BYTE> value = this->outPi.GetRawData((this->outPi.GetSize() * 8), 0);

		UINT row = 0;
		UINT col = 0;
		QTableWidgetItem *cell = NULL;
		// qDebug("Output size %d", value.size());
		for (std::vector<BYTE>::const_iterator it = value.begin();
				it != value.end(); ++it)
		{
			bool isSortenabled = this->ui.outTable->isSortingEnabled();
			this->ui.outTable->setSortingEnabled(false);

			cell = this->ui.outTable->item(row, col);
			if (cell)
				cell->setText(QString("%1").arg(*it, 0, 16).rightJustified(2, '0'));

			this->ui.outTable->setSortingEnabled(isSortenabled);

			if ((col + 1) == this->ui.outTable->columnCount())
			{
				++row;
				col = 0;
			}
			else
			{
				++col;
			}
		}
	}
	catch(const std::exception& ex)
	{
		qDebug("An Exception has occured: %s", ex.what());
	}
}

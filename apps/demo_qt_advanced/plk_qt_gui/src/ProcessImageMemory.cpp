/**
********************************************************************************
\file   ProcessImageMemory.cpp

\brief  Implements the memory view of the input and output processimage data
from the oplk stack using the Qt 5.2 QTableWidgets.

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
#include "ProcessImageMemory.h"
#include "oplk/oplk.h"

/*******************************************************************************
* Public functions
*******************************************************************************/
ProcessImageMemory::ProcessImageMemory(ProcessImageIn &in, ProcessImageOut &out, QWidget *parent) :
	QFrame(parent),
	inPi(in),
	outPi(out)
{
	this->ui.setupUi(this);

	const UINT inputRowCount = (in.GetSize() / this->ui.inputTable->columnCount()) + 1;
	const UINT outputRowCount = (out.GetSize() / this->ui.outTable->columnCount()) + 1;
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
	for (UINT row = 0; row < this->ui.inputTable->rowCount(); ++row)
	{
		for (UINT col = 0; col < this->ui.inputTable->columnCount(); ++col)
		{
			item = this->ui.inputTable->item(row, col);
			if (item != NULL)
			{
				delete item;
			}
		}
	}

	for (UINT row = 0; row < this->ui.outTable->rowCount(); ++row)
	{
		for (UINT col = 0; col < this->ui.outTable->columnCount(); ++col)
		{
			item = this->ui.outTable->item(row, col);
			if (item != NULL)
			{
				delete item;
			}
		}
	}

	for (UINT row = 0; row < this->ui.inputTable->rowCount(); ++row)
	{
		item = this->ui.inputTable->verticalHeaderItem(row);
		if (item != NULL)
		{
			delete item;
		}
	}

	for (UINT col = 0; col < this->ui.inputTable->columnCount(); ++col)
	{
		item = this->ui.inputTable->horizontalHeaderItem(col);
		if (item != NULL)
		{
			delete item;
		}
	}

	for (UINT row = 0; row < this->ui.outTable->rowCount(); ++row)
	{
		item = this->ui.outTable->verticalHeaderItem(row);
		if (item != NULL)
		{
			delete item;
		}
	}

	for (UINT col = 0; col < this->ui.outTable->columnCount(); ++col)
	{
		item = this->ui.outTable->horizontalHeaderItem(col);
		if (item != NULL)
		{
			delete item;
		}
	}
}

/*******************************************************************************
* Private functions
*******************************************************************************/
void ProcessImageMemory::CreateVerticalHeaders()
{
	QTableWidgetItem *item = NULL;
	for (UINT i = 0; i < ((this->inPi.GetSize() / this->ui.inputTable->columnCount()) + 1);
		 ++i)
	{
		item = new QTableWidgetItem();
		item->setText((QString("%1").arg(i * 16, 0, 16)).rightJustified(4, '0'));
		this->ui.inputTable->setVerticalHeaderItem(i, item);
	}

	for (UINT i = 0; i < ((this->outPi.GetSize() / this->ui.outTable->columnCount()) + 1);
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
	for (UINT row = 0; row < this->ui.inputTable->rowCount(); ++row)
	{
		for (UINT col = 0; col <this->ui.inputTable->columnCount(); ++col)
		{
			item = new QTableWidgetItem();
			item->setFont(QFont("Courier", 9));
			this->ui.inputTable->setItem(row, col, item);
		}
	}

	for (UINT row = 0; row < this->ui.outTable->rowCount(); ++row)
	{
		for (UINT col = 0; col <this->ui.outTable->columnCount(); ++col)
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

		for (std::vector<BYTE>::const_iterator it = value.begin();
				it != value.end(); ++it)
		{
			const bool isSortenabled = this->ui.inputTable->isSortingEnabled();
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
		// TODO Discuss about exposing the error to the user.
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

		for (std::vector<BYTE>::const_iterator it = value.begin();
				it != value.end(); ++it)
		{
			const bool isSortenabled = this->ui.outTable->isSortingEnabled();
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
		// TODO Discuss about exposing the error to the user.
		qDebug("An Exception has occured: %s", ex.what());
	}
}

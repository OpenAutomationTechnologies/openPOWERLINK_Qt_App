/**
********************************************************************************
\file   ProcessImageMemory.cpp

\brief  Implements the memory view of the input and output processimage data
from the OPlk stack using the Qt 5.2 QTableWidgets.

\todo
		- Input validation for InputProcessImage cells while editing.

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

#include <QtCore/QLocale>

#include "api/OplkQtApi.h"

/*******************************************************************************
* Public functions
*******************************************************************************/
ProcessImageMemory::ProcessImageMemory(QWidget *parent) :
	QWidget(parent),
	inPi(NULL),
	outPi(NULL),
	processImageInputSlotIndex(this->metaObject()->indexOfMethod(
								QMetaObject::normalizedSignature(
								"UpdateFromInputValues()").constData())),
	processImageOutputSlotIndex(this->metaObject()->indexOfMethod(
								QMetaObject::normalizedSignature(
								"UpdateFromOutputValues()").constData()))
{
	this->ui.setupUi(this);
	Q_ASSERT(processImageInputSlotIndex != -1);
	Q_ASSERT(processImageOutputSlotIndex != -1);
}

ProcessImageMemory::~ProcessImageMemory()
{
}

void ProcessImageMemory::ResetView()
{
	if (this->inPi)
	{
		bool ret = OplkQtApi::UnregisterSyncEventHandler(Direction::PI_IN,
										 *(this),
										 this->metaObject()->method(this->processImageInputSlotIndex));
		Q_ASSERT(ret != false);
	}

	if (this->outPi)
	{
		bool ret = OplkQtApi::UnregisterSyncEventHandler(Direction::PI_OUT,
										 *(this),
										 this->metaObject()->method(this->processImageOutputSlotIndex));
		Q_ASSERT(ret != false);
	}

	this->inPi = NULL;
	this->outPi = NULL;

	while (this->ui.inputTable->rowCount() > 0)
	{
		this->ui.inputTable->removeRow(0);
	}

	while (this->ui.outTable->rowCount() > 0)
	{
		this->ui.outTable->removeRow(0);
	}

	this->ui.inputTable->update();
	this->ui.outTable->update();
}

void ProcessImageMemory::SetProcessImage(ProcessImageIn *inPi, const ProcessImageOut *outPi)
{
	if (inPi)
	{
		this->inPi = inPi;

		const UINT inputRowCount = (this->inPi->GetSize() / this->ui.inputTable->columnCount()) + 1;
		this->ui.inputTable->setRowCount(inputRowCount);
		this->ui.inputTable->verticalHeader()->setVisible(true);
		this->CreateVerticalHeaders(Direction::PI_IN);
		this->CreateCells(Direction::PI_IN);
		bool ret = OplkQtApi::RegisterSyncEventHandler(Direction::PI_IN,
											 *(this),
											 this->metaObject()->method(this->processImageInputSlotIndex));
		Q_ASSERT(ret != false);
		this->ui.inputTable->update();
	}

	if (outPi)
	{
		this->outPi = outPi;

		const UINT outputRowCount = (this->outPi->GetSize() / this->ui.outTable->columnCount()) + 1;
		this->ui.outTable->setRowCount(outputRowCount);
		this->ui.outTable->verticalHeader()->setVisible(true);
		this->CreateVerticalHeaders(Direction::PI_OUT);
		this->CreateCells(Direction::PI_OUT);
		bool ret = OplkQtApi::RegisterSyncEventHandler(Direction::PI_OUT,
											 *(this),
											 this->metaObject()->method(this->processImageOutputSlotIndex));
		Q_ASSERT(ret != false);
		this->ui.outTable->update();
	}
}

/*******************************************************************************
* Private functions
*******************************************************************************/
void ProcessImageMemory::CreateVerticalHeaders(Direction::Direction direction)
{
	if (direction == Direction::PI_IN)
	{
		QTableWidgetItem *item = NULL;
		for (UINT i = 0; i < ((this->inPi->GetSize() / this->ui.inputTable->columnCount()) + 1);
			 ++i)
		{
			item = new QTableWidgetItem();
			item->setText((QString("%1")
						   .arg(i * this->ui.inputTable->columnCount(), 0, 16))
						   .rightJustified(4, '0'));
			this->ui.inputTable->setVerticalHeaderItem(i, item);
		}
	}

	if (direction == Direction::PI_OUT)
	{
		QTableWidgetItem *item = NULL;
		for (UINT i = 0; i < ((this->outPi->GetSize() / this->ui.outTable->columnCount()) + 1);
			 ++i)
		{
			item = new QTableWidgetItem();
			item->setFont(QFont("Courier", 9));
			item->setText((QString("%1")
						   .arg(i * this->ui.outTable->columnCount(), 0, 16))
						   .rightJustified(4, '0'));
			this->ui.outTable->setVerticalHeaderItem(i, item);
		}
	}
}

void ProcessImageMemory::CreateCells(Direction::Direction direction)
{
	if (direction == Direction::PI_IN)
	{
		QTableWidgetItem *item = NULL;
		for (UINT row = 0; row < this->ui.inputTable->rowCount(); ++row)
		{
			for (UINT col = 0; col < this->ui.inputTable->columnCount(); ++col)
			{
				item = new QTableWidgetItem();
				item->setFont(QFont("Courier", 9));
				this->ui.inputTable->setItem(row, col, item);

				// Block editing of extra cells.
				if (!IsValidCell(row, col, Direction::PI_IN))
					item->setFlags(Qt::NoItemFlags);
			}
		}
	}

	if (direction == Direction::PI_OUT)
	{
		QTableWidgetItem *item = NULL;
		for (UINT row = 0; row < this->ui.outTable->rowCount(); ++row)
		{
			for (UINT col = 0; col < this->ui.outTable->columnCount(); ++col)
			{
				item = new QTableWidgetItem();
				item->setFont(QFont("Courier", 9));
				this->ui.outTable->setItem(row, col, item);

				// Block editing of extra cells.
				if (!IsValidCell(row, col, Direction::PI_OUT))
					item->setFlags(Qt::NoItemFlags);
			}
		}
	}
}

void ProcessImageMemory::ResizeColumnsToContents()
{
	this->ui.inputTable->resizeColumnsToContents();
	this->ui.outTable->resizeColumnsToContents();
}

void ProcessImageMemory::UpdateFromInputValues()
{
	if (this->inPi)
	{
		try
		{
			std::vector<BYTE> value = this->inPi->GetRawData((this->inPi->GetSize() * 8), 0);
			UINT row = 0;
			UINT col = 0;
			QTableWidgetItem *cell = NULL;

			for (std::vector<BYTE>::const_iterator it = value.begin();
					it != value.end(); ++it)
			{
				cell = this->ui.inputTable->item(row, col);
				if (cell)
					cell->setText((QString("%1").arg(*it, 0, 16).rightJustified(2, '0'))
								  .toUpper());

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
			qDebug("An Exception has occurred: %s", ex.what());
		}
	}
}

void ProcessImageMemory::UpdateFromOutputValues()
{
	if (this->outPi)
	{
		try
		{
			std::vector<BYTE> value = this->outPi->GetRawData((this->outPi->GetSize() * 8), 0);

			UINT row = 0;
			UINT col = 0;
			QTableWidgetItem *cell = NULL;

			for (std::vector<BYTE>::const_iterator it = value.begin();
					it != value.end(); ++it)
			{
				cell = this->ui.outTable->item(row, col);
				if (cell)
					cell->setText((QString("%1").arg(*it, 0, 16).rightJustified(2, '0'))
								  .toUpper());

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
			qDebug("An Exception has occurred: %s", ex.what());
		}
	}
}

void ProcessImageMemory::on_inputTable_itemChanged(QTableWidgetItem *cell)
{
	if (this->inPi)
	{
		try
		{
			UINT row = cell->row();
			UINT col = cell->column();

			// Avoid setting to extra cells.
			if (!IsValidCell(row, col, Direction::PI_IN))
				return;

			bool ok = false;
			UINT horiz = this->ui.inputTable->horizontalHeaderItem(col)->text().toUInt(&ok, 16);
			UINT vertic = this->ui.inputTable->verticalHeaderItem(row)->text().toUInt(&ok, 16);

			std::vector<BYTE> value;
			value.push_back((BYTE) cell->text().toUInt(&ok, 16));
			this->inPi->SetRawData(value, (horiz + vertic));
		}
		catch(const std::exception& ex)
		{
			qDebug("on_inputTable_itemChanged: An Exception has occurred: %s", ex.what());
		}
	}
}

bool ProcessImageMemory::IsValidCell(UINT row, UINT col, Direction::Direction direction)
{
	bool valid = true;
	if (direction == Direction::PI_IN)
	{
		if (!this->inPi)
			valid = false;

		if ((row == (this->ui.inputTable->rowCount() - 1))
			&& (col >= (this->ui.inputTable->columnCount()
				- (this->inPi->GetSize() % this->ui.inputTable->columnCount()))))
			valid = false;
	}
	else if (direction == Direction::PI_OUT)
	{
		if (!this->outPi)
			valid = false;

		if ((row == (this->ui.outTable->rowCount() - 1))
			&& (col >= (this->ui.outTable->columnCount()
				- (this->outPi->GetSize() % this->ui.outTable->columnCount()))))
			valid = false;
	}
	else
	{
		valid = false;
	}

	return valid;
}

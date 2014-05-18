/**
********************************************************************************
\file   ProcessImageMemory.h

\brief  Describes the memory view of the input and output processimage data
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

#ifndef _PROCESSIMAGE_MEMORY_H_
#define _PROCESSIMAGE_MEMORY_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "ui_ProcessImageMemory.h"

#include "user/processimage/ProcessImageIn.h"
#include "user/processimage/ProcessImageOut.h"

/**
 * \brief The ProcessImageMemory class inherits the QWidget and describes the
 * logic to update the input and output processimage data in a memory view format
 * by using the QTableWidgets.
 */
class ProcessImageMemory : public QWidget
{
	Q_OBJECT

public:
	/**
	 * \brief Constructs the ProcessImageMemory instance and prepares the
	 * Table based on the size of the ProcessImageIn and ProcessImageOut.
	 *
	 * \param[in] parent
	 */
	explicit ProcessImageMemory(QWidget *parent = 0);

	~ProcessImageMemory();

public slots:

	/**
	 * \brief Sets the input and output processimage instance thereby
	 * prepares the input and output variables view.
	 *
	 * \param inPi Input processimage instance.
	 * \param outPi Output processimage instance.
	 */
	void SetProcessImage(ProcessImageIn *inPi, const ProcessImageOut *outPi);

	/**
	 * \brief Updates the processimage input table value from and/or to the stack.
	 */
	void UpdateFromInputValues();

	/**
	 * \brief Update the processimage output table value from the stack.
	 */
	void UpdateFromOutputValues();

	/**
	 * \brief Resets the contents of the input and output table and invalidates
	 * the processimage pointer.
	 */
	void ResetView();

private slots:
	/**
	 * \brief Triggers editing input processimage cells.
	 *
	 * Overrides the value to the processimage data.
	 *
	 * \param cell The pointer to the cell's widget.
	 */
	void on_inputTable_itemChanged(QTableWidgetItem *cell);

private:
	Ui::ProcessImageMemory ui;     ///< ProcessImage memory view Ui instance.

	ProcessImageIn *inPi;          ///< Input processimage instance.
	const ProcessImageOut *outPi;  ///< Output processimage instance.

	const int processImageInputSlotIndex;
	const int processImageOutputSlotIndex;

	/**
	 * \brief Creates the vertical headers for the input and output
	 * processimage table.
	 *
	 * \param[in] direction Direction of the processimage table.
	 */
	void CreateVerticalHeaders(Direction::Direction direction);

	/**
	 * \brief Creates the cells needed dynamically depending on the size of the
	 * input and output processimage.
	 *
	 * \param[in] direction Direction of the processimage table.
	 */
	void CreateCells(Direction::Direction direction);

	/**
	 * \brief Resize the input and output processimage table's columns
	 *  to contents width.
	 */
	void ResizeColumnsToContents();

	/**
	 * \brief Checks for the valid cell or is just a padded cell.
	 *
	 * \param[in] row Id of the row.
	 * \param[in] col Id of the column.
	 * \param[in] direction Direction of the processimage table.
	 * \retval true  If the cell is valid.
	 * \retval false If the cell is not valied.
	 */
	bool IsValidCell(UINT row, UINT col, Direction::Direction direction);
};

#endif // _PROCESSIMAGE_MEMORY_H_

/**
********************************************************************************
\file   LineEditWidget.h

\brief  Provides an user-interface instance of an LineEdit by inheriting Qt 5.2 LineEdit.

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

#ifndef _LINEEDIT_WIDGET_H_
#define _LINEEDIT_WIDGET_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <QtWidgets/QLineEdit>

/**
 * \brief Re-implemented from QLineEdit to handle the focus events.
 */
class LineEditWidget : public QLineEdit
{
	Q_OBJECT

public:
	explicit LineEditWidget(QWidget* parent = 0);

signals:
	/**
	 * \brief Signal to announce that the LineEdit got the focus.
	 */
	void SignalFocusIn();

	/**
	 * \brief Signal to announce that the Lined is out of focus.
	 */
	void SignalFocusOut();

protected:
	/**
	 * \brief Re-Implemented from QLineEdit::focusInEvent()
	 * \param[in] event The generated event on LineEdit focus out.
	 */
	virtual void focusInEvent(QFocusEvent* event);

	/**
	 * \brief Re-Implemented from QLineEdit::focusOutEvent()
	 * \param[in] event The generated event on LineEdit focus out.
	 */
	virtual void focusOutEvent(QFocusEvent* event);
};

#endif // _LINEEDIT_WIDGET_H_

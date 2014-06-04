/**
********************************************************************************
\file   OplkEventHandler.h

\brief  Design of an event handler that uses QThread to
		communicate openPOWERLINK asynchronous callback events via QT signals

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

#ifndef _OPLK_STACK_MONITOR_H_
#define _OPLK_STACK_MONITOR_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/

#include <QtCore/QThread>
#include <QtCore/QString>

/**
 * \brief The OplkMonitorStack class
 */
class OplkMonitorStack : public QThread
{
Q_OBJECT

private:

	friend class OplkQtApi;

	OplkMonitorStack();
	OplkMonitorStack(const OplkMonitorStack& eventHandler);
	OplkMonitorStack& operator=(const OplkMonitorStack& eventHandler);

	/**
	 * \return Returns the instance of the class
	 */
	static OplkMonitorStack& GetInstance();

protected:
	virtual ~OplkMonitorStack();
	virtual void run();

signals:
	/**
	 * \brief This signal is emitted when there is a critical error in the stack occurred.
	 *
	 * \param[in] errorMessage Detailed information about the error.
	 */
	void SignalCriticalError(const QString& errorMessage);
};

#endif // _OPLK_STACK_MONITOR_H_

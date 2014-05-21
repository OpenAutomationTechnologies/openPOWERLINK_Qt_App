/**
********************************************************************************
\file   OplkSyncEventHandler.h

\brief  Describes the transfer of processimage data in a thread by using
		Qt 5.2 threads.

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

#ifndef _OPLK_SYNC_EVENT_HANDLER_H_
#define _OPLK_SYNC_EVENT_HANDLER_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/

#include <QThread>
#include <oplk/errordefs.h>
#include <oplk/event.h>

#include "api/OplkQtApi.h"

/**
 * \brief The OplkSyncEventHandler class
 *
 * Describes the thread to transfer synchronous processimage data.
 *
 * \note This class is intended to _only_ be used by OplkQtApi
 * \note The signals can be received by registering through the
 * OplkQtApi::RegisterSyncEventHandler() functions
 */
class OplkSyncEventHandler : public QThread
{
	Q_OBJECT

public:
	virtual ~OplkSyncEventHandler();

signals:

	/**
	 * \brief Signals that the input processimage is about to be updated.
	 *
	 * Receivers of this signal are being notified that values in the
	 * input processimage will be transferred to the openPOWERLINK stack.
	 */
	void SignalUpdateInputValues();

	/**
	 * \brief Signals that the output processimage has been updated.
	 *
	 * Receivers of this signal are being notified that values in the
	 * output processimage have been updated by the openPOWERLINK stack.
	 */
	void SignalUpdatedOutputValues();

	/**
	 * \brief Signal notifies that the wait time has been changed.
	 * \param[in] waitTime Time in micro seconds.
	 */
	void SignalSyncWaitTimeChanged(ulong waitTime);
	// Use only ulong QT does not connect with ULONG or unsigned long or unsigned long it.

protected:
	/**
	 * \brief The reimplemented function of run.
	 */
	virtual void run();

private:

	friend class OplkQtApi;

	ULONG sleepTime; ///< Thread sleep time in micro seconds.

	OplkSyncEventHandler();
	OplkSyncEventHandler(const OplkSyncEventHandler& syncThread);
	OplkSyncEventHandler& operator=(const OplkSyncEventHandler& syncThread);

	/**
	 * \return The instance of the class
	 */
	static OplkSyncEventHandler& GetInstance();

	/**
	 * \brief AppCbSync
	 * \return a tOplkError error code.
	 */
	static tOplkError AppCbSync(void);

	/**
	 * \return The pointer to the callback function AppCbSync().
	 */
	tSyncCb GetCbSync() const;

	/**
	 * \brief Process the processimage data transfer.
	 * \return a tOplkError error code.
	 */
	tOplkError ProcessSyncEvent();

	/**
	 * \return Sleep time in micro seconds.
	 */
	ULONG GetSleepTime() const;

	/**
	 * \param[in] sleepTime Time in micro seconds.
	 */
	void SetSleepTime(const ULONG sleepTime);
};

#endif // _OPLK_SYNC_EVENT_HANDLER_H_


/**
********************************************************************************
\file   DataSyncThread.h

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

#ifndef _DATA_SYNC_THREAD_H_
#define _DATA_SYNC_THREAD_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/

#include <QThread>
#include <oplk/errordefs.h>
#include <oplk/event.h>

#include "api/OplkQtApi.h"

/**
 * \brief The DataSyncThread class
 *
 * Describes the thread to transfer synchronous processimage data.
 */
class DataSyncThread : public QThread
{
	Q_OBJECT

public:
	virtual ~DataSyncThread();

	/**
	 * \return The address of the synchronous data callback function.
	 */
	tSyncCb GetSyncCbFunc();

	ULONG GetSleepMsecs(void);

	void SetSleepMsecs(ULONG mSecs);

signals:
	/**
	 * \brief A signal to UpdateInputValues
	 */
	void SignalUpdateInputValues();

	/**
	 * \brief A signal to UpdateOutputValues
	 */
	void SignalUpdateOutputValues();

private:

	friend class OplkQtApi;

	ULONG sleepMicroSeconds;

	DataSyncThread();
	DataSyncThread(const DataSyncThread& syncThread);
	DataSyncThread& operator=(const DataSyncThread& syncThread);

	/**
	 * \return Returns the instance of the class
	 */
	static DataSyncThread& GetInstance();

	/**
	 * \brief The reimplemented function of run.
	 */
	virtual void run();

	/**
	 * \brief Process the processimage data transfer.
	 * \return a tOplkError error code.
	 */
	tOplkError ProcessSync();

	/**
	 * \brief AppCbSync
	 * \return a tOplkError error code.
	 */
	static tOplkError AppCbSync(void);
};

#endif // _DATA_SYNC_THREAD_H_


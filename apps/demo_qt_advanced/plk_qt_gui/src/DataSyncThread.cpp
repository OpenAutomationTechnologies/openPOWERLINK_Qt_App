/**
********************************************************************************
\file   DataSyncThread.cpp

\brief  Implements the transfer of processimage data in a thread by
		using Qt 5.2 threads.

\todo
		- Implement mutex and wait condition before the thread terminates it.
		- Export the refresh-rate as a settings

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
#include "DataSyncThread.h"

#include "api/OplkQtApi.h"


/*******************************************************************************
* Public functions
*******************************************************************************/

DataSyncThread::DataSyncThread()
{

}

DataSyncThread::~DataSyncThread()
{

}

/*******************************************************************************
* Private functions
*******************************************************************************/

void DataSyncThread::run()
{
	for (;;)
	{
		tOplkError oplkRet = kErrorGeneralError;
		oplkRet = DataSyncThread::ProcessSync();
		if (oplkRet != kErrorOk)
		{
			return;
		}
		QThread::msleep(500);
	}
}

tOplkError DataSyncThread::ProcessSync()
{
	tOplkError oplkRet = kErrorGeneralError;
	oplkRet = oplk_exchangeProcessImageOut();
	if (oplkRet != kErrorOk)
	{
		return oplkRet;
	}

	emit SignalUpdateInputValues();
	emit SignalUpdateOutputValues();

	oplkRet = oplk_exchangeProcessImageIn();
	return oplkRet;
}

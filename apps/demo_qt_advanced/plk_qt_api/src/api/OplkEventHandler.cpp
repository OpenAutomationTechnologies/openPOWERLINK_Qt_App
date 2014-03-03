/**
********************************************************************************
\file   OplkEventHandler.cpp

\brief  Implementation of a event handler class that uses QThread to
		communicate openPOWERLINK stack events via QT signals

Handles the events from the eventcallback function from the stack and
triggers the signals to the receiver who has requested for it.

/*------------------------------------------------------------------------------
Copyright (c) 2014, Kalycito Infotech Private Limited
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
------------------------------------------------------------------------------*/

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <QDateTime>
#include <QString>
#include <QMetaMethod>
#include <oplk/debugstr.h>

#include "api/OplkEventHandler.h"

/*******************************************************************************
* PUBLIC Functions
*******************************************************************************/
void OplkEventHandler::AwaitNmtGsOff()
{
	mutex.lock();
	nmtGsOffCondition.wait(&mutex);
	mutex.unlock();
}

tEplApiCbEvent OplkEventHandler::GetEventCbFunc(void)
{
	return AppCbEvent;
}

/*******************************************************************************
* PRIVATE Functions
*******************************************************************************/

OplkEventHandler::OplkEventHandler(){}

OplkEventHandler& OplkEventHandler::GetInstance()
{
	// Local static object - Not thread safe
	static OplkEventHandler instance;
	return instance;
}

tOplkError OplkEventHandler::AppCbEvent(tEplApiEventType eventType,
								tEplApiEventArg* eventArg,
								void* userArg)
{
	tOplkError  oplkRet = kErrorOk;

	switch (eventType)
	{
		case kEplApiEventUserDef:
			//Nothing to do
			break;

		case kEplApiEventNmtStateChange:
			oplkRet = OplkEventHandler::GetInstance().ProcessNmtStateChangeEvent(
										&eventArg->m_NmtStateChange, userArg);
			break;

	//    case kEplApiEventRequestNmt:
	//        //Nothing to do
	//        break;

		case kEplApiEventCriticalError:
		case kEplApiEventWarning:
			oplkRet = OplkEventHandler::GetInstance().ProcessErrorWarningEvent(
										&eventArg->m_InternalError, userArg);
			break;

		case kEplApiEventHistoryEntry:
			oplkRet = OplkEventHandler::GetInstance().ProcessHistoryEvent(
										&eventArg->m_ErrHistoryEntry, userArg);
			break;

		case kEplApiEventNode:
			oplkRet = OplkEventHandler::GetInstance().ProcessNodeEvent(
										&eventArg->m_Node, userArg);
			break;

		case kEplApiEventBoot:
			//Nothing to do
			break;

		case kEplApiEventSdo:
			oplkRet = OplkEventHandler::GetInstance().ProcessSdoEvent(
										&eventArg->m_Sdo, userArg);
			break;

		case kEplApiEventObdAccess:
			//Nothing to do
			break;

		case kEplApiEventLed:
			//Nothing to do
			break;

		case kEplApiEventCfmProgress:
			oplkRet = OplkEventHandler::GetInstance().ProcessCfmProgressEvent(
										&eventArg->m_CfmProgress, userArg);
			break;

		case kEplApiEventCfmResult:
			oplkRet = OplkEventHandler::GetInstance().ProcessCfmResultEvent(
										&eventArg->m_CfmResult, userArg);
			break;

		case kEplApiEventReceivedAsnd:
			//Nothing to do
			break;

		default:
			break;
	}

	return oplkRet;
}

//TODO: C4711: Think of inlining.
void OplkEventHandler::TriggerLocalNodeStateChanged(tNmtState nmtState)
{
	emit SignalLocalNodeStateChanged(nmtState);
}

void OplkEventHandler::TriggerNodeFound(const int nodeId)
{
	emit SignalNodeFound(nodeId);
}

void OplkEventHandler::TriggerNodeStateChanged(const int nodeId,
							tNmtState nmtState)
{
	emit SignalNodeStateChanged(nodeId, nmtState);
}

void OplkEventHandler::TriggerSdoTransferFinished(const tSdoComFinished& result,
							const ReceiverContext* receiverContext)
{
	SdoTransferResult sdoTransferResult = SdoTransferResult(result.nodeId,
											result.targetIndex,
											result.targetSubIndex,
											result.transferredBytes,
											result.sdoAccessType,
											result.sdoComConState,
											result.abortCode);
//    result.pUserArg
	emit SignalSdoTransferFinished(sdoTransferResult);
	qDebug("Signal emitted Abort code: %x", result.abortCode);

	oplk_freeSdoChannel(result.sdoComConHdl);
	qDebug("freed SdoChannel");

	bool conSuccessful = false;

	conSuccessful = QObject::disconnect(&OplkEventHandler::GetInstance(),
						QMetaMethod::fromSignal(&OplkEventHandler::SignalSdoTransferFinished),
						receiverContext->GetReceiver(),
						*(receiverContext->GetReceiverFunction()));
	qDebug("disconnected %d", conSuccessful);

	delete receiverContext;

	/*
	 * TODO(RaM): Delete the handle if success
	 * */
}

void OplkEventHandler::TriggerPrintLog(QString logStr)
{
	QString str;

	str.append(QDateTime::currentDateTime().toString("yyyy/MM/dd-hh:mm:ss.zzz"));
	str.append(" - ");
	str.append(logStr);

	emit SignalPrintLog(str);
}

tOplkError OplkEventHandler::ProcessNmtStateChangeEvent(
								tEventNmtStateChange* nmtStateChange,
								void* userArg)
{
	tOplkError oplkRet = kErrorOk;

	UNUSED_PARAMETER(userArg);

	TriggerLocalNodeStateChanged(nmtStateChange->newNmtState);

	switch (nmtStateChange->newNmtState)
	{
		case kNmtGsOff:

			// NMT state machine was shut down,
			// because of user signal (CTRL-C) or critical EPL stack error
			// also shut down EplApiProcess()
			oplkRet = kErrorShutdown;
			// and unblock OplkEventHandler thread
			oplk_freeProcessImage(); //jba do we need it here?

			TriggerPrintLog(QString("NMTStateChangeEvent(0x%1) originating event = 0x%2 (%3)")
				 .arg(nmtStateChange->newNmtState, 0, 16, QLatin1Char('0'))
				 .arg(nmtStateChange->nmtEvent, 0, 16, QLatin1Char('0'))
				 .arg(debugstr_getNmtEventStr(nmtStateChange->nmtEvent)));
			mutex.lock();
			nmtGsOffCondition.wakeAll();
			mutex.unlock();
			break;

		case kNmtGsResetCommunication:
			TriggerPrintLog(QString("StateChangeEvent(0x%1) originating event = 0x%2 (%3)")
				 .arg(nmtStateChange->newNmtState, 4, 16, QLatin1Char('0'))
				 .arg(nmtStateChange->nmtEvent, 4, 16, QLatin1Char('0'))
				 .arg(debugstr_getNmtEventStr(nmtStateChange->nmtEvent)));
			break;

		case kNmtGsResetConfiguration:
			TriggerPrintLog(QString("StateChangeEvent(0x%1) originating event = 0x%2 (%3)")
				 .arg(nmtStateChange->newNmtState, 4, 16, QLatin1Char('0'))
				 .arg(nmtStateChange->nmtEvent, 4, 16, QLatin1Char('0'))
				 .arg(debugstr_getNmtEventStr(nmtStateChange->nmtEvent)));
			break;

		case kNmtGsInitialising:
		case kNmtGsResetApplication:
		case kNmtCsNotActive:
		case kNmtCsPreOperational1:
		case kNmtCsPreOperational2:
		case kNmtCsReadyToOperate:
		case kNmtCsBasicEthernet:
		case kNmtMsNotActive:
		case kNmtMsPreOperational1:
		case kNmtMsPreOperational2:
		case kNmtMsReadyToOperate:
		case kNmtMsBasicEthernet:
			TriggerPrintLog(QString("StateChangeEvent(0x%1) originating event = 0x%2 (%3)")
				 .arg(nmtStateChange->newNmtState, 4, 16, QLatin1Char('0'))
				 .arg(nmtStateChange->nmtEvent, 4, 16, QLatin1Char('0'))
				 .arg(debugstr_getNmtEventStr(nmtStateChange->nmtEvent)));
			break;

		case kNmtCsOperational:
		case kNmtMsOperational:
			TriggerPrintLog(QString("StateChangeEvent(0x%1) originating event = 0x%2 (%3)")
				 .arg(nmtStateChange->newNmtState, 4, 16, QLatin1Char('0'))
				 .arg(nmtStateChange->nmtEvent, 4, 16, QLatin1Char('0'))
				 .arg(debugstr_getNmtEventStr(nmtStateChange->nmtEvent)));
			break;

		case kNmtCsStopped:
			//TODO
			break;

		default:
			break;
	}

	return oplkRet;
}

tOplkError OplkEventHandler::ProcessErrorWarningEvent(
								tEplEventError* internalError,
								void* userArg)
{

	UNUSED_PARAMETER(userArg);

	TriggerPrintLog(QString("Err/Warn: Source = %1 (0x%2) EplError = %3 (0x%4)")
		.arg(debugstr_getEventSourceStr(internalError->m_EventSource))
		.arg(internalError->m_EventSource, 2, 16, QLatin1Char('0'))
		.arg(debugstr_getRetValStr(internalError->m_EplError))
		.arg(internalError->m_EplError, 3, 16, QLatin1Char('0')));

	switch (internalError->m_EventSource)
	{
		case kEplEventSourceEventk:
		case kEplEventSourceEventu:
			// error occurred within event processing
			// either in kernel or in user part
			TriggerPrintLog(QString(" OrgSource = %1 %2")
				 .arg(debugstr_getEventSourceStr(internalError->m_Arg.m_EventSource))
				 .arg(internalError->m_Arg.m_EventSource, 2, 16, QLatin1Char('0')));
			break;

		case kEplEventSourceDllk:
			// error occurred within the data link layer (e.g. interrupt processing)
			// the ULONG argument contains the DLL state and the NMT event
			TriggerPrintLog(QString(" val = %1").arg(internalError->m_Arg.m_dwArg, 0, 16));
			break;

		default:
			break;
	}
	return kErrorOk;
}

tOplkError OplkEventHandler::ProcessHistoryEvent(
								tErrHistoryEntry* historyEntry,
								void* userArg)
{

	UNUSED_PARAMETER(userArg);

	TriggerPrintLog(QString("HistoryEntry: Type=0x%1 Code=0x%2 (0x%3 %4 %5 %6 %7 %8 %9 %10)")
		.arg(historyEntry->m_wEntryType, 4, 16, QLatin1Char('0'))
		.arg(historyEntry->m_wErrorCode, 4, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->m_abAddInfo[0], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->m_abAddInfo[1], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->m_abAddInfo[2], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->m_abAddInfo[3], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->m_abAddInfo[4], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->m_abAddInfo[5], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->m_abAddInfo[6], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->m_abAddInfo[7], 2, 16, QLatin1Char('0')));

	return kErrorOk;
}

tOplkError OplkEventHandler::ProcessNodeEvent(tEplApiEventNode* nodeEvent,
								void* userArg)
{
	tOplkError oplkRet = kErrorOk;

	UNUSED_PARAMETER(userArg);

	switch (nodeEvent->m_NodeEvent)
	{
		case kNmtNodeEventCheckConf:
			TriggerPrintLog(QString("Node Event: (Node=%2, CheckConf)") .arg(nodeEvent->m_uiNodeId, 0, 10));
			break;

		case kNmtNodeEventUpdateConf:
			TriggerPrintLog(QString("Node Event: (Node=%1, UpdateConf)") .arg(nodeEvent->m_uiNodeId, 0, 10));
			break;

		case kNmtNodeEventFound:
			TriggerNodeFound(nodeEvent->m_uiNodeId);
			break;

		case kNmtNodeEventNmtState:
		{
			switch (nodeEvent->m_NmtState)
			{
				case kNmtGsOff:
				case kNmtGsInitialising:
				case kNmtGsResetApplication:
				case kNmtGsResetCommunication:
				case kNmtGsResetConfiguration:
				case kNmtCsNotActive:
					TriggerNodeStateChanged(nodeEvent->m_uiNodeId, nodeEvent->m_NmtState);
					break;

				case kNmtCsPreOperational1:
				case kNmtCsPreOperational2:
				case kNmtCsReadyToOperate:
					//d.p.: Do we need this? Is the stack not forwarding the 2nd IdentResponse after
					//a NmtResetConf? Testing
					TriggerNodeFound(nodeEvent->m_uiNodeId);
					TriggerNodeStateChanged(nodeEvent->m_uiNodeId, nodeEvent->m_NmtState);
					break;

				case kNmtCsOperational:
					TriggerNodeStateChanged(nodeEvent->m_uiNodeId, nodeEvent->m_NmtState);
					break;

				case kNmtCsBasicEthernet:
				case kNmtCsStopped:
				default:
					TriggerNodeStateChanged(nodeEvent->m_uiNodeId, nodeEvent->m_NmtState);
					break;
			}
			break;
		}

		case kNmtNodeEventError:
		{
		//??nodeEvent->m_NmtState?      TriggerNodeStateChanged(nodeEvent->m_uiNodeId, -1);
			TriggerPrintLog(QString("AppCbEvent (Node=%1): Error = %2 (0x%3)")
				.arg(nodeEvent->m_uiNodeId, 0, 10)
				.arg(debugstr_getEmergErrCodeStr(nodeEvent->m_wErrorCode))
				.arg(nodeEvent->m_wErrorCode, 4, 16, QLatin1Char('0')));
			break;
		}
		default:
			break;
	}
	oplkRet = kErrorOk;
	return oplkRet;
}

tOplkError OplkEventHandler::ProcessSdoEvent(tSdoComFinished* sdoEvent,
								void* userArg)
{
	UNUSED_PARAMETER(userArg);
	qDebug("ProcessSDO: %d", sdoEvent->sdoComConState);

	switch (sdoEvent->sdoComConState)
	{
		case kEplSdoComTransferNotActive:
			break;
		case kEplSdoComTransferRunning:
			//Segmented transfer - (eg. CFM and segmented transfer)
			break;
		case kEplSdoComTransferTxAborted:
		case kEplSdoComTransferRxAborted:
		case kEplSdoComTransferFinished:
		case kEplSdoComTransferLowerLayerAbort:
		{
			TriggerSdoTransferFinished(*sdoEvent, (const ReceiverContext*)sdoEvent->pUserArg);
			break;
		}
		default:
			break;
	}
	return kErrorOk;
}

tOplkError OplkEventHandler::ProcessCfmProgressEvent(
								tCfmEventCnProgress* cfmProgress,
								void* userArg)
{
	UNUSED_PARAMETER(userArg);

	TriggerPrintLog(QString("CFM Progress: (Node=%1, CFM-Progress: Object 0x%2/%3,  %4/%5 Bytes")
			 .arg(cfmProgress->nodeId, 0, 10)
			 .arg(cfmProgress->objectIndex, 4, 16, QLatin1Char('0'))
			 .arg(cfmProgress->objectSubIndex, 0, 10)
			 .arg((ULONG)cfmProgress->bytesDownloaded, 0, 10)
			 .arg((ULONG)cfmProgress->totalNumberOfBytes, 0, 10));

	if ((cfmProgress->sdoAbortCode != 0)
		|| (cfmProgress->error != kErrorOk))
	{
		 TriggerPrintLog(QString("	-> SDO Abort=0x%1, Error=0x%2)")
				 .arg((ULONG) cfmProgress->sdoAbortCode, 0, 16 , QLatin1Char('0'))
				 .arg(cfmProgress->error, 0, 16, QLatin1Char('0')));
	}
	return kErrorOk;
}

tOplkError OplkEventHandler::ProcessCfmResultEvent(
								tEplApiEventCfmResult* cfmResult,
								void* userArg)
{
	UNUSED_PARAMETER(userArg);

	switch (cfmResult->m_NodeCommand)
	{
		case kNmtNodeCommandConfOk:
			TriggerPrintLog(QString("CFM Result: (Node=%1, ConfOk)").arg(cfmResult->m_uiNodeId, 0, 10));
			break;
		case kNmtNodeCommandConfErr:
			TriggerPrintLog(QString("CFM Result: (Node=%1, ConfErr)").arg(cfmResult->m_uiNodeId, 0, 10));
			break;
		case kNmtNodeCommandConfReset:
			TriggerPrintLog(QString("CFM Result: (Node=%1, ConfReset)").arg(cfmResult->m_uiNodeId, 0, 10));
			break;
		case kNmtNodeCommandConfRestored:
			TriggerPrintLog(QString("CFM Result: (Node=%1, ConfRestored)").arg(cfmResult->m_uiNodeId, 0, 10));
			break;
		case kNmtNodeCommandBoot:
			TriggerPrintLog(QString("CFM Result: (Node=%1, BootCommand)").arg(cfmResult->m_uiNodeId, 0, 10));
			break;
		case kNmtNodeCommandSwOk:
			TriggerPrintLog(QString("CFM Result: (Node=%1, Sw-Ok)").arg(cfmResult->m_uiNodeId, 0, 10));
			break;
		case kNmtNodeCommandSwUpdated:
			TriggerPrintLog(QString("CFM Result: (Node=%1, Sw-Updated)").arg(cfmResult->m_uiNodeId, 0, 10));
			break;
		case kNmtNodeCommandStart:
			TriggerPrintLog(QString("CFM Result: (Node=%1, NodeStart)").arg(cfmResult->m_uiNodeId, 0, 10));
			break;
		default:
			TriggerPrintLog(QString("CFM Result: (Node=%d, CfmResult=0x%X)")
					.arg(cfmResult->m_uiNodeId, 0, 10)
					.arg(cfmResult->m_NodeCommand, 4, 16, QLatin1Char('0')));
			break;
	}
	return kErrorOk;
}


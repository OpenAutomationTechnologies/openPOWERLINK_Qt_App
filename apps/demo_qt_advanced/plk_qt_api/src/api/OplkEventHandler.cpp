/**
********************************************************************************
\file   OplkEventHandler.cpp

\brief  Implementation of a event handler class that uses QThread to
		communicate openPOWERLINK stack events via QT signals

Handles the events from the eventcallback function from the stack and
triggers the signals to the receiver who has requested for it.

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
	OplkEventHandler::mutex.lock();
	OplkEventHandler::nmtGsOffCondition.wait(&(OplkEventHandler::mutex));
	OplkEventHandler::mutex.unlock();
}

tOplkApiCbEvent OplkEventHandler::GetAppEventCbFunc(void)
{
	return AppCbEvent;
}

void OplkEventHandler::run()
{
	// start process function
	tOplkError oplkRet = oplk_process();
	//TODO Handle reurn values.
	Q_ASSERT(oplkRet == kErrorOk);
}

/*******************************************************************************
* PRIVATE Functions
*******************************************************************************/
OplkEventHandler::OplkEventHandler()
{
}

OplkEventHandler& OplkEventHandler::GetInstance()
{
	// Local static object - Not thread safe
	static OplkEventHandler instance;
	return instance;
}

tOplkError OplkEventHandler::AppCbEvent(tOplkApiEventType eventType,
								tOplkApiEventArg* eventArg,
								void* userArg)
{
	tOplkError oplkRet = kErrorGeneralError;

	switch (eventType)
	{
		case kOplkApiEventNmtStateChange:
			oplkRet = OplkEventHandler::GetInstance().ProcessNmtStateChangeEvent(
										&eventArg->nmtStateChange, userArg);
			break;

		case kOplkApiEventCriticalError:
			oplkRet = OplkEventHandler::GetInstance().ProcessCriticalErrorEvent(
										&eventArg->internalError, userArg);
			break;

		case kOplkApiEventWarning:
			oplkRet = OplkEventHandler::GetInstance().ProcessWarningEvent(
									&eventArg->internalError, userArg);
			break;

		case kOplkApiEventHistoryEntry:
			oplkRet = OplkEventHandler::GetInstance().ProcessHistoryEvent(
										&eventArg->errorHistoryEntry, userArg);
			break;

		case kOplkApiEventNode:
			oplkRet = OplkEventHandler::GetInstance().ProcessNodeEvent(
										&eventArg->nodeEvent, userArg);
			break;

		case kOplkApiEventSdo:
			oplkRet = OplkEventHandler::GetInstance().ProcessSdoEvent(
										&eventArg->sdoInfo, userArg);
			break;

		case kOplkApiEventCfmProgress:
			oplkRet = OplkEventHandler::GetInstance().ProcessCfmProgressEvent(
										&eventArg->cfmProgress, userArg);
			break;

		case kOplkApiEventCfmResult:
			oplkRet = OplkEventHandler::GetInstance().ProcessCfmResultEvent(
										&eventArg->cfmResult, userArg);
			break;
		case kOplkApiEventPdoChange:
			oplkRet = OplkEventHandler::GetInstance().ProcessPdoChangeEvent(
										&eventArg->pdoChange, userArg);
			break;

		// case kOplkApiEventRequestNmt:
		case kOplkApiEventUserDef:
		case kOplkApiEventBoot:
		case kOplkApiEventObdAccess:
		case kOplkApiEventLed:
		case kOplkApiEventReceivedAsnd:
			// Ignored events.
			oplkRet = kErrorOk;
			break;
		default:
			qDebug("%s  Default case", __FUNCTION__);
			break;
	}

	return oplkRet;
}

//TODO: C4711: Think of inlining.
void OplkEventHandler::TriggerLocalNodeStateChanged(tNmtState nmtState)
{
	emit OplkEventHandler::SignalLocalNodeStateChanged(nmtState);
}

void OplkEventHandler::TriggerNodeFound(const int nodeId)
{
	emit OplkEventHandler::SignalNodeFound(nodeId);
}

void OplkEventHandler::TriggerNodeStateChanged(const int nodeId,
							tNmtState nmtState)
{
	emit OplkEventHandler::SignalNodeStateChanged(nodeId, nmtState);
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

	emit OplkEventHandler::SignalSdoTransferFinished(sdoTransferResult);

	tOplkError oplkRet = oplk_freeSdoChannel(result.sdoComConHdl);
	if (oplkRet != kErrorOk)
	{
		// TODO throw? freeSdoChannel err.
		qDebug("free SDO channel fail. Err: 0x%x", oplkRet);
	}

	if(receiverContext)
	{
		bool disconnected = QObject::disconnect(&OplkEventHandler::GetInstance(),
							QMetaMethod::fromSignal(&OplkEventHandler::SignalSdoTransferFinished),
							receiverContext->GetReceiver(),
							*(receiverContext->GetReceiverFunction()));
		if (!disconnected)
		{
			qDebug("Disconnect failed ! RetVal %d", disconnected);
			/* This should not happen concerning the implementation of OplkQtApi::TransferObject
			 * TODO report error.
			 */
		}
		else
		{
			qDebug("Disconnected Success");
		}
		delete receiverContext;
	}
	else
	{
		qDebug("NULL Receiver Context. Not disconnected");
	}
}

void OplkEventHandler::TriggerPrintLog(QString logStr)
{
	QString str;

	str.append(QDateTime::currentDateTime().toString("yyyy/MM/dd-hh:mm:ss.zzz"));
	str.append(" - ");
	str.append(logStr);

	emit OplkEventHandler::SignalPrintLog(str);
}

void OplkEventHandler::TriggerCriticalError(const QString errorMessage)
{
	emit this->SignalCriticalError(errorMessage);
}

tOplkError OplkEventHandler::ProcessNmtStateChangeEvent(
								tEventNmtStateChange* nmtStateChange,
								void* userArg)
{
	tOplkError oplkRet = kErrorGeneralError;

	UNUSED_PARAMETER(userArg);

	OplkEventHandler::TriggerLocalNodeStateChanged(nmtStateChange->newNmtState);

	switch (nmtStateChange->newNmtState)
	{
		case kNmtGsOff:
		{
			// NMT state machine was shut down,
			// because of user signal (CTRL-C) or critical EPL stack error
			// also shut down stack
			oplkRet = kErrorShutdown;

//			oplk_freeProcessImage(); //jba do we need it here?

			OplkEventHandler::TriggerPrintLog(
				QString("NMTStateChangeEvent(0x%1) originating event = 0x%2 (%3)")
				 .arg(nmtStateChange->newNmtState, 0, 16, QLatin1Char('0'))
				 .arg(nmtStateChange->nmtEvent, 0, 16, QLatin1Char('0'))
				 .arg(debugstr_getNmtEventStr(nmtStateChange->nmtEvent)));

			// unblock OplkEventHandler thread
			OplkEventHandler::mutex.lock();
			OplkEventHandler::nmtGsOffCondition.wakeAll();
			OplkEventHandler::mutex.unlock();
			break;
		}

		case kNmtGsResetCommunication:
		case kNmtGsResetConfiguration:
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
		case kNmtCsOperational:
		case kNmtMsOperational:
		case kNmtCsStopped:
		{
			OplkEventHandler::TriggerPrintLog(
				QString("StateChangeEvent(0x%1) originating event = 0x%2 (%3)")
				 .arg(nmtStateChange->newNmtState, 4, 16, QLatin1Char('0'))
				 .arg(nmtStateChange->nmtEvent, 4, 16, QLatin1Char('0'))
				 .arg(debugstr_getNmtEventStr(nmtStateChange->nmtEvent)));
			oplkRet = kErrorOk;
			break;
		}

		default:
			qDebug("%s  Default case", __FUNCTION__);
			break;
	}

	return oplkRet;
}

tOplkError OplkEventHandler::ProcessCriticalErrorEvent(
								tEventError* internalError,
								void* userArg)
{
	UNUSED_PARAMETER(userArg);

	QString error = QString("Error source = %1 (0x%2) oplkError = %3 (0x%4)")
			.arg(debugstr_getEventSourceStr(internalError->eventSource))
			.arg(internalError->eventSource, 2, 16, QLatin1Char('0'))
			.arg(debugstr_getRetValStr(internalError->oplkError))
			.arg(internalError->oplkError, 3, 16, QLatin1Char('0'));

	OplkEventHandler::TriggerPrintLog(error);

	switch (internalError->eventSource)
	{
		case kEventSourceEventk:
		case kEventSourceEventu:
		{
			// error occurred within event processing either in kernel or in user part
			this->TriggerCriticalError(QString("%1, OrgSource: %2(0x%3)")
									   .arg(error)
									   .arg(debugstr_getEventSourceStr(internalError->errorArg.eventSource))
									   .arg(internalError->errorArg.eventSource, 2, 16, QLatin1Char('0')));
			OplkEventHandler::TriggerPrintLog(QString(" OrgSource = %1(0x%2)")
				.arg(debugstr_getEventSourceStr(internalError->errorArg.eventSource))
				.arg(internalError->errorArg.eventSource, 2, 16, QLatin1Char('0')));
			break;
		}
		case kEventSourceDllk:
			// error occurred within the data link layer (e.g. interrupt processing)
			// the uintArg argument contains the DLL state and the NMT event
			this->TriggerCriticalError(QString("%1, value: %2")
									   .arg(error)
									   .arg(internalError->errorArg.uintArg, 0, 16));

			OplkEventHandler::TriggerPrintLog(QString(" val = %1")
				.arg(internalError->errorArg.uintArg, 0, 16));
			break;

	// other errors are considered as warnings and are ignored as per stack 2.0

		default:
			qDebug("%s  Default case", __FUNCTION__);
			break;
	}
	return kErrorOk;
}

tOplkError OplkEventHandler::ProcessWarningEvent(tEventError* internalError,
									void* userArg)
{

	UNUSED_PARAMETER(userArg);

	OplkEventHandler::TriggerPrintLog(
		QString("Warning: source = %1 (0x%2) oplkError = %3 (0x%4)")
		.arg(debugstr_getEventSourceStr(internalError->eventSource))
		.arg(internalError->eventSource, 2, 16, QLatin1Char('0'))
		.arg(debugstr_getRetValStr(internalError->oplkError))
		.arg(internalError->oplkError, 3, 16, QLatin1Char('0')));

	// TODO: process internal structure of tEventError to provide appropriate warning messages.

	return kErrorOk;
}

tOplkError OplkEventHandler::ProcessHistoryEvent(
								tErrHistoryEntry* historyEntry,
								void* userArg)
{
	UNUSED_PARAMETER(userArg);

	OplkEventHandler::TriggerPrintLog(
		QString("HistoryEntry: Type=0x%1 Code=0x%2 (0x%3 %4 %5 %6 %7 %8 %9 %10)")
		.arg(historyEntry->entryType, 4, 16, QLatin1Char('0'))
		.arg(historyEntry->errorCode, 4, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->aAddInfo[0], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->aAddInfo[1], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->aAddInfo[2], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->aAddInfo[3], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->aAddInfo[4], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->aAddInfo[5], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->aAddInfo[6], 2, 16, QLatin1Char('0'))
		.arg((WORD)historyEntry->aAddInfo[7], 2, 16, QLatin1Char('0')));

	return kErrorOk;
}

tOplkError OplkEventHandler::ProcessNodeEvent(tOplkApiEventNode* nodeEvent,
								void* userArg)
{
	UNUSED_PARAMETER(userArg);

	switch (nodeEvent->nodeEvent)
	{
		case kNmtNodeEventCheckConf:
			OplkEventHandler::TriggerPrintLog(
					QString("Node Event: (Node=%2, CheckConf)")
					.arg(nodeEvent->nodeId));
			break;

		case kNmtNodeEventUpdateConf:
			OplkEventHandler::TriggerPrintLog(
					QString("Node Event: (Node=%1, UpdateConf)")
					.arg(nodeEvent->nodeId));
			break;

		case kNmtNodeEventFound:
			OplkEventHandler::TriggerNodeFound(nodeEvent->nodeId);
			break;

		case kNmtNodeEventNmtState:
			OplkEventHandler::TriggerNodeStateChanged(nodeEvent->nodeId, nodeEvent->nmtState);
			OplkEventHandler::TriggerPrintLog(
					QString("Node Event: (Node=%1, State: %2)")
					.arg(nodeEvent->nodeId)
					.arg(debugstr_getNmtStateStr(nodeEvent->nmtState)));
			break;

		case kNmtNodeEventError:
		{
			OplkEventHandler::TriggerPrintLog(
					QString("AppCbEvent (Node=%1): Error = %2 (0x%3)")
					.arg(nodeEvent->nodeId)
					.arg(debugstr_getEmergErrCodeStr(nodeEvent->errorCode))
					.arg(nodeEvent->errorCode, 4, 16, QLatin1Char('0')));
			break;
		}
		default:
			qDebug("%s  Default case", __FUNCTION__);
			break;
	}
	return kErrorOk;
}

tOplkError OplkEventHandler::ProcessSdoEvent(tSdoComFinished* sdoEvent,
								void* userArg)
{
	tOplkError oplkRet = kErrorGeneralError;
	UNUSED_PARAMETER(userArg);
	qDebug("ProcessSDO: %d", sdoEvent->sdoComConState);

	switch (sdoEvent->sdoComConState)
	{
		case kSdoComTransferTxAborted:
		case kSdoComTransferRxAborted:
		case kSdoComTransferFinished:
		case kSdoComTransferLowerLayerAbort:
		{
			OplkEventHandler::TriggerSdoTransferFinished(*sdoEvent,
						(const ReceiverContext*)sdoEvent->pUserArg);
			oplkRet = kErrorOk;
			break;
		}

		case kSdoComTransferNotActive:
		case kSdoComTransferRunning:
		{
			//Segmented transfer
			oplkRet = kErrorOk;
			break;
		}
		default:
			qDebug("%s  Default case", __FUNCTION__);
			break;
	}
	return oplkRet;
}

tOplkError OplkEventHandler::ProcessCfmProgressEvent(
								tCfmEventCnProgress* cfmProgress,
								void* userArg)
{
	UNUSED_PARAMETER(userArg);

	OplkEventHandler::TriggerPrintLog(
			QString("CFM Progress: (Node=%1, CFM-Progress: Object 0x%2/%3,  %4/%5 Bytes")
			.arg(cfmProgress->nodeId)
			.arg(cfmProgress->objectIndex, 4, 16, QLatin1Char('0'))
			.arg(cfmProgress->objectSubIndex)
			.arg((ULONG)cfmProgress->bytesDownloaded)
			.arg((ULONG)cfmProgress->totalNumberOfBytes));

	if ((cfmProgress->sdoAbortCode != 0)
		|| (cfmProgress->error != kErrorOk))
	{
		 OplkEventHandler::TriggerPrintLog(
			QString("	-> SDO Abort=0x%1, Error=0x%2)")
			.arg((ULONG) cfmProgress->sdoAbortCode, 0, 16 , QLatin1Char('0'))
			.arg(cfmProgress->error, 0, 16, QLatin1Char('0')));
	}

	return kErrorOk;
}

tOplkError OplkEventHandler::ProcessCfmResultEvent(
								tOplkApiEventCfmResult* cfmResult,
								void* userArg)
{
	UNUSED_PARAMETER(userArg);

	switch (cfmResult->nodeCommand)
	{
		case kNmtNodeCommandConfOk:
			OplkEventHandler::TriggerPrintLog(
						QString("CFM Result: (Node=%1, ConfOk)")
						.arg(cfmResult->nodeId));
			break;
		case kNmtNodeCommandConfErr:
			OplkEventHandler::TriggerPrintLog(
						QString("CFM Result: (Node=%1, ConfErr)")
						.arg(cfmResult->nodeId));
			break;
		case kNmtNodeCommandConfReset:
			OplkEventHandler::TriggerPrintLog(
						QString("CFM Result: (Node=%1, ConfReset)")
						.arg(cfmResult->nodeId));
			break;
		case kNmtNodeCommandConfRestored:
			OplkEventHandler::TriggerPrintLog(
						QString("CFM Result: (Node=%1, ConfRestored)")
						.arg(cfmResult->nodeId));
			break;
		case kNmtNodeCommandBoot:
			OplkEventHandler::TriggerPrintLog(
						QString("CFM Result: (Node=%1, BootCommand)")
						.arg(cfmResult->nodeId));
			break;
		case kNmtNodeCommandSwOk:
			OplkEventHandler::TriggerPrintLog(
						QString("CFM Result: (Node=%1, Sw-Ok)")
						.arg(cfmResult->nodeId));
			break;
		case kNmtNodeCommandSwUpdated:
			OplkEventHandler::TriggerPrintLog(
						QString("CFM Result: (Node=%1, Sw-Updated)")
						.arg(cfmResult->nodeId));
			break;
		case kNmtNodeCommandStart:
			OplkEventHandler::TriggerPrintLog(
						QString("CFM Result: (Node=%1, NodeStart)")
						.arg(cfmResult->nodeId));
			break;
		default:
			OplkEventHandler::TriggerPrintLog(
						QString("CFM Result: (Node=%d, CfmResult=0x%X)")
						.arg(cfmResult->nodeId)
					.arg(cfmResult->nodeCommand, 4, 16, QLatin1Char('0')));
			break;
	}
	return kErrorOk;
}

tOplkError OplkEventHandler::ProcessPdoChangeEvent(tOplkApiEventPdoChange* pdoChange,
												void* userArg)
{
	UINT64                      mappObject;
	UINT varLen        = sizeof(mappObject);
	tOplkError oplkRet = kErrorGeneralError;

	UNUSED_PARAMETER(userArg);

	OplkEventHandler::TriggerPrintLog(
		QString("PDO change event: (%1PDO = 0x%2 to node 0x%3 with %4 objects %5)")
				.arg(pdoChange->fTx ? "T" : "R")
				.arg(pdoChange->mappParamIndex, 4, 16, QLatin1Char('0'))
				.arg(pdoChange->nodeId, 2, 16, QLatin1Char('0'))
				.arg(pdoChange->mappObjectCount)
				.arg(pdoChange->fActivated ? "activated" : "deleted"));

	for (UINT subIndex = 1; subIndex <= pdoChange->mappObjectCount; ++subIndex)
	{
		oplkRet = oplk_readLocalObject(pdoChange->mappParamIndex, subIndex, &mappObject, &varLen);
		if (oplkRet != kErrorOk)
		{
			OplkEventHandler::TriggerPrintLog(
						QString("  Reading 0x%1/%2 failed with 0x%3")
						.arg(pdoChange->mappParamIndex, 4, 16, QLatin1Char('0'))
						.arg(subIndex)
						.arg(oplkRet, 4, 16, QLatin1Char('0')));
			continue;
		}

		OplkEventHandler::TriggerPrintLog(
					QString("  %1. mapped object 0x%2/%3")
					.arg(subIndex)
					.arg(mappObject & 0x00FFFFULL, 4, 16, QLatin1Char('0'))
					.arg((mappObject & 0xFF0000ULL) >> 16, 4, 16, QLatin1Char('0')));
	}

	return kErrorOk;
}

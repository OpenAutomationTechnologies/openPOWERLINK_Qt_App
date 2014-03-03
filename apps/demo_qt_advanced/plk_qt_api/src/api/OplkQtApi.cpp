/**
********************************************************************************
\file   OplkQtApi.cpp

\brief  Contains the implementions to wrap the openPOWERLINK APIs.

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

#include "api/OplkQtApi.h"

const ULONG kIpAddress = 0xc0a86401;   /**< 192.168.100.1 */
const ULONG kSubnetMask = 0xFFFFFF00;  /**< 255.255.255.0 */
const std::string kHostName = "openPOWERLINK Stack";
const std::string kIfEth = EPL_VETH_NAME;
const UINT kCycleLen = 5000;  /**< Cycle Length (0x1006: NMT_CycleLen_U32) in [us] */
const BYTE abMacAddr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  /**< Default MAC Address */

static char* defaultCDCFilename = (char *)"mnobd.cdc";  /**< CDC file name */
static bool cdcSet = false;

tEplApiInitParam OplkQtApi::initParam; /**< initparam */

void OplkQtApi::SetInitParam()
{
	EPL_MEMSET(&initParam, 0, sizeof (initParam));
	initParam.m_uiSizeOfStruct = sizeof (initParam);

	initParam.m_fAsyncOnly = FALSE;

	initParam.m_dwFeatureFlags = UINT_MAX;      // 0x1F82: NMT_FeatureFlags_U32
	initParam.m_dwCycleLen = kCycleLen;         // required for error detection
	initParam.m_uiIsochrTxMaxPayload = 256;     // const
	initParam.m_uiIsochrRxMaxPayload = 256;     // const
	initParam.m_dwPresMaxLatency = 50000;       // const; only required for IdentRes
	initParam.m_uiPreqActPayloadLimit = 36;     // required for initialisation (+28 bytes)
	initParam.m_uiPresActPayloadLimit = 36;     // required for initialisation of Pres frame (+28 bytes)
	initParam.m_dwAsndMaxLatency = 150000;      // const; only required for IdentRes
	initParam.m_uiMultiplCycleCnt = 0;          // required for error detection
	initParam.m_uiAsyncMtu = 1500;              // required to set up max frame size
	initParam.m_uiPrescaler = 2;                // required for sync
	initParam.m_dwLossOfFrameTolerance = 500000;
	initParam.m_dwAsyncSlotTimeout = 3000000;
	initParam.m_dwWaitSocPreq = 150000;
	initParam.m_dwDeviceType = UINT_MAX;        // NMT_DeviceType_U32
	initParam.m_dwVendorId = UINT_MAX;          // NMT_IdentityObject_REC.VendorId_U32
	initParam.m_dwProductCode = UINT_MAX;       // NMT_IdentityObject_REC.ProductCode_U32
	initParam.m_dwRevisionNumber = UINT_MAX;    // NMT_IdentityObject_REC.RevisionNo_U32
	initParam.m_dwSerialNumber = UINT_MAX;      // NMT_IdentityObject_REC.SerialNo_U32

	initParam.m_dwSubnetMask = kSubnetMask;
	initParam.m_dwDefaultGateway = 0;
	EPL_MEMCPY(initParam.m_sHostname, kHostName.c_str(), sizeof(initParam.m_sHostname));
	initParam.m_uiSyncNodeId = EPL_C_ADR_SYNC_ON_SOA;
	initParam.m_fSyncOnPrcNode = FALSE;

	// write 00:00:00:00:00:00 to MAC address, so that the driver uses the real hardware address
	EPL_MEMCPY(initParam.m_abMacAddress, abMacAddr, sizeof(initParam.m_abMacAddress));

	// set callback functions
	initParam.m_pfnCbEvent = OplkEventHandler::GetInstance().GetEventCbFunc();
	if (initParam.m_pfnCbEvent == NULL)
	{
		qDebug("Null Call back");
	}
}

tOplkError OplkQtApi::InitStack(const UINT nodeId,
						const std::string& networkInterface)
{
	tOplkError oplkRet = kErrorOk;

	OplkQtApi::SetInitParam();

	initParam.m_uiNodeId = nodeId;
	initParam.m_dwIpAddress = (kIpAddress & kSubnetMask) | initParam.m_uiNodeId;

	initParam.m_HwParam.m_pszDevName = networkInterface.c_str();

	oplkRet = oplk_init(&initParam);
	if (oplkRet != kErrorOk)
	{
		qDebug("Ret: %d",oplkRet);
		goto Exit;
	}

Exit:
	 return oplkRet;
}

tOplkError OplkQtApi::StartStack()
{
	tOplkError oplkRet = kErrorOk;

	if (cdcSet == false)
	{
		qDebug("Set default cdc file name");
		oplkRet = oplk_setCdcFilename(defaultCDCFilename);
		if (oplkRet != kErrorOk)
		{
			qDebug("setDefault Cdc File Ret: %d",oplkRet);
			goto Exit;
		}
	}

	oplkRet = oplk_setupProcessImage();
	if (oplkRet != kErrorOk)
	{
		qDebug("SetupProcessImage retCode %x", oplkRet);
		goto Exit;
	}

	// Starting process thread
	OplkEventHandler::GetInstance().start();

	// Start the OPLK stack by sending NMT s/w reset
	oplkRet = oplk_execNmtCommand(kNmtEventSwReset);
	if (oplkRet != kErrorOk)
	{
		qDebug("kNmtEventSwReset Ret: %d", oplkRet);
	}

Exit:
	return oplkRet;
}

tOplkError OplkQtApi::StopStack()
{
	tOplkError oplkRet = kErrorOk;

	oplkRet = oplk_execNmtCommand(kNmtEventSwitchOff);
	if (oplkRet != kErrorOk)
	{
		qDebug("kNmtEventSwitchOff Ret: %d", oplkRet);
	}

	OplkEventHandler::GetInstance().AwaitNmtGsOff();
	// OplkEventHandler::GetInstance().terminate();
	oplkRet = oplk_freeProcessImage();
	if (oplkRet != kErrorOk)
	{
		qDebug("freeProcessImage Ret: %d", oplkRet);
	}

	oplkRet = oplk_shutdown();
	if (oplkRet != kErrorOk)
	{
		qDebug("shutdown Ret: %d", oplkRet);
	}
	return oplkRet;
}

bool OplkQtApi::RegisterNodeFoundEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	return QObject::connect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalNodeFound),
		&receiver,
		receiverFunction,
		(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection)
	);
}

bool OplkQtApi::UnregisterNodeFoundEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	return QObject::connect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalNodeFound),
		&receiver,
		receiverFunction
	);
}

bool OplkQtApi::RegisterNodeStateChangedEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	qRegisterMetaType<tNmtState>("tNmtState");
	return QObject::connect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalNodeStateChanged),
		&receiver,
		receiverFunction,
		(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection)
	);
}

bool OplkQtApi::UnregisterNodeStateChangedEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	qRegisterMetaType<tNmtState>("tNmtState");
	return QObject::disconnect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalNodeStateChanged),
		&receiver,
		receiverFunction
	);
}

bool OplkQtApi::RegisterLocalNodeStateChangedEventHandler(
					const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	qRegisterMetaType<tNmtState>("tNmtState");
	return QObject::connect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalLocalNodeStateChanged),
		&receiver,
		receiverFunction,
		(Qt::ConnectionType)(Qt::QueuedConnection | Qt::UniqueConnection)
	);
}

bool OplkQtApi::UnregisterLocalNodeStateChangedEventHandler(
					const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	qRegisterMetaType<tNmtState>("tNmtState");
	return QObject::disconnect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalLocalNodeStateChanged),
		&receiver,
		receiverFunction
	);
}

bool OplkQtApi::RegisterEventLogger(const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	return QObject::connect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalPrintLog),
		&receiver,
		receiverFunction,
		(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection)
	);
}

bool OplkQtApi::UnregisterEventLogger(const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	return QObject::disconnect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalPrintLog),
		&receiver,
		receiverFunction
	);
}

tOplkError OplkQtApi::ExecuteNmtCommand(const UINT nodeId,
						tNmtCommand nmtCommand)
{
	return oplk_execRemoteNmtCommand(nodeId, nmtCommand);

}

tOplkError OplkQtApi::TransferObject(const SdoTransferJob& sdoTransferJob,
						const QObject& receiver,
						const QMetaMethod& receiverFunction)
{
	tSdoComConHdl *sdoComConHdl = new tSdoComConHdl;
	UINT dataSize =  sdoTransferJob.GetDataSize();
	bool conSuccess = false;
	ReceiverContext *receiverContext = new ReceiverContext(&receiver, &receiverFunction);

	qRegisterMetaType<SdoTransferResult>("SdoTransferResult");

	if ( ((initParam.m_uiNodeId == sdoTransferJob.GetNodeId())
		 || (0 == sdoTransferJob.GetNodeId())) )
	{
		// do not connect
		qDebug("Not Connected");
	}
	else
	{
		conSuccess = QObject::connect(&OplkEventHandler::GetInstance(),
					QMetaMethod::fromSignal(&OplkEventHandler::SignalSdoTransferFinished),
					&receiver,
					receiverFunction,
					(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection)
		);
		qDebug("Read: %d", conSuccess);
	}

	tOplkError oplkRet = kErrorOk;
	switch (sdoTransferJob.GetSdoAccessType())
	{
		case kSdoAccessTypeRead:
		{
			oplkRet = oplk_readObject(sdoComConHdl,
						sdoTransferJob.GetNodeId(),
						sdoTransferJob.GetIndex(),
						sdoTransferJob.GetSubIndex(),
						sdoTransferJob.GetData(),
						&dataSize,
						sdoTransferJob.GetSdoType(),
						(void*) receiverContext);
			break;
		}
		case kSdoAccessTypeWrite:
		{
			qDebug("Write Val %x", (sdoTransferJob.GetData()));
			oplkRet = oplk_writeObject(sdoComConHdl,
						sdoTransferJob.GetNodeId(),
						sdoTransferJob.GetIndex(),
						sdoTransferJob.GetSubIndex(),
						sdoTransferJob.GetData(),
						dataSize,
						sdoTransferJob.GetSdoType(),
						(void*) receiverContext);
			break;
		}
		default:
			qDebug("Error Case: API: TransferObject ");
			break;
	}

	if ( (oplkRet == kErrorApiTaskDeferred)
		|| ((sdoTransferJob.GetNodeId() == initParam.m_uiNodeId)
		|| (sdoTransferJob.GetNodeId() == 0)) )
	{
		//Should not disconnect. Success Case
		qDebug("Not Disconnected");
	}
	else
	{
		conSuccess = QObject::disconnect(&OplkEventHandler::GetInstance(),
					QMetaMethod::fromSignal(&OplkEventHandler::SignalSdoTransferFinished),
					&receiver,
					receiverFunction
		);
		qDebug("Read Err :Disconnected %d", conSuccess);
	}
	return oplkRet;
}

tOplkError OplkQtApi::AllocateProcessImage(ProcessImageIn& in,
						ProcessImageOut& out)
{
	tOplkError oplkRet = kErrorOk;
	oplkRet = oplk_allocProcessImage(in.GetSize(), out.GetSize());
	if (oplkRet != kErrorOk)
	{
		qDebug("allocProcessImage retCode %x", oplkRet);
		return oplkRet;
	}

	in.SetProcessImageDataPtr((BYTE*)oplk_getProcessImageIn());
	out.SetProcessImageDataPtr((BYTE*)oplk_getProcessImageOut());

	return oplkRet;
}

void OplkQtApi::SetCdc(const BYTE* cdcBuffer, const UINT size)
{
	tOplkError oplkRet = kErrorOk;
	oplkRet = oplk_setCdcBuffer((BYTE*) cdcBuffer, size);
	cdcSet = true;
}

void OplkQtApi::SetCdc(const char* cdcFileName)
{
	tOplkError oplkRet = kErrorOk;
	oplkRet = oplk_setCdcFilename((char*) cdcFileName);
	cdcSet = true;
}

void OplkQtApi::SetCycleTime(const ULONG cycleTime)
{
	tOplkError oplkRet = kErrorOk;

	oplkRet = oplk_writeLocalObject(0x1006, 0x00, (void*)&cycleTime, 4);

	// If this is a CN. It has to do remote SDO write?.
}

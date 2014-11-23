/**
********************************************************************************
\file   OplkQtApi.cpp

\brief  Contains the implementations to wrap the openPOWERLINK APIs.

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
#include <QtCore/QMetaType>
#include <QtCore/QtDebug>

#include "api/OplkQtApi.h"
#include "api/OplkEventHandler.h"
#include "api/OplkSyncEventHandler.h"

/*******************************************************************************
* Module global variables
*******************************************************************************/
static const ULONG kIpAddress = 0xc0a864F0;   ///< MN by default (192.168.100.240)
static const ULONG kSubnetMask = 0xFFFFFF00;  ///< 255.255.255.0
static const std::string kHostName = "openPOWERLINK Stack"; ///< max 32 chars
static const ULONG kCycleLen = 5000;  ///< Cycle Length (0x1006: NMT_CycleLen_U32) in [us]
static const BYTE abkMacAddr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  ///<Default MAC Address
static const std::string kDefaultCdcFilename = CONFIG_OBD_DEF_CONCISEDCF_FILENAME;  ///< Default CDC file name


/*******************************************************************************
* Static member variables
*******************************************************************************/
tOplkApiInitParam OplkQtApi::initParam; ///< initparam.
bool OplkQtApi::cdcSet = false;  ///< Flag to detect CDC has been set or not.

/*******************************************************************************
* Private functions
*******************************************************************************/

void OplkQtApi::SetInitParam()
{
	OPLK_MEMSET(&OplkQtApi::initParam, 0, sizeof (OplkQtApi::initParam));
	OplkQtApi::initParam.sizeOfInitParam = sizeof (OplkQtApi::initParam);

	OplkQtApi::initParam.fAsyncOnly = FALSE;

	OplkQtApi::initParam.featureFlags = UINT_MAX;      // 0x1F82: NMT_FeatureFlags_U32
	OplkQtApi::initParam.cycleLen = kCycleLen;         // required for error detection
	OplkQtApi::initParam.isochrTxMaxPayload = 256;     // const
	OplkQtApi::initParam.isochrRxMaxPayload = 256;     // const
	OplkQtApi::initParam.presMaxLatency = 50000;       // const; only required for IdentRes
	OplkQtApi::initParam.preqActPayloadLimit = 36;     // required for initialisation (+28 bytes)
	OplkQtApi::initParam.presActPayloadLimit = 36;     // required for initialisation of PRes frame (+28 bytes)
	OplkQtApi::initParam.asndMaxLatency = 150000;      // const; only required for IdentRes
	OplkQtApi::initParam.multiplCylceCnt = 0;          // required for error detection
	OplkQtApi::initParam.asyncMtu = 1500;              // required to set up max frame size
	OplkQtApi::initParam.prescaler = 2;                // required for sync
	OplkQtApi::initParam.lossOfFrameTolerance = 500000;
	OplkQtApi::initParam.asyncSlotTimeout = 3000000;
	OplkQtApi::initParam.waitSocPreq = 150000;
	OplkQtApi::initParam.deviceType = UINT_MAX;        // NMT_DeviceType_U32
	OplkQtApi::initParam.vendorId = UINT_MAX;          // NMT_IdentityObject_REC.VendorId_U32
	OplkQtApi::initParam.productCode = UINT_MAX;       // NMT_IdentityObject_REC.ProductCode_U32
	OplkQtApi::initParam.revisionNumber = UINT_MAX;    // NMT_IdentityObject_REC.RevisionNo_U32
	OplkQtApi::initParam.serialNumber = UINT_MAX;      // NMT_IdentityObject_REC.SerialNo_U32

	OplkQtApi::initParam.subnetMask = kSubnetMask;
	OplkQtApi::initParam.defaultGateway = 0;
	OPLK_MEMCPY(OplkQtApi::initParam.sHostname, kHostName.c_str(), sizeof(OplkQtApi::initParam.sHostname));
	OplkQtApi::initParam.syncNodeId = C_ADR_SYNC_ON_SOA;
	OplkQtApi::initParam.fSyncOnPrcNode = FALSE;

	// write 00:00:00:00:00:00 to MAC address, so that the driver uses the real hardware address
	OPLK_MEMCPY(OplkQtApi::initParam.aMacAddress, abkMacAddr, sizeof(OplkQtApi::initParam.aMacAddress));

	// set callback functions
	OplkQtApi::initParam.pfnCbEvent = OplkEventHandler::GetInstance().GetAppEventCbFunc();
	if (OplkQtApi::initParam.pfnCbEvent == NULL)
	{
		qDebug("Null Call back");
		// Never expecting to happen
		// If needed Throw std err or return kErrorInvalidInstanceParam
	}

#if defined(CONFIG_KERNELSTACK_DIRECTLINK)
	OplkQtApi::initParam.pfnCbSync = OplkSyncEventHandler::GetInstance().GetCbSync();
#else
	OplkQtApi::initParam.pfnCbSync = NULL;
#endif

	OplkQtApi::initParam.pEventUserArg = NULL;
//	OplkQtApi::initParam.hwParam.devNum = 0;
//	OplkQtApi::initParam.syncResLatency = 0;


	OplkQtApi::initParam.pDevName = NULL;       // NMT_ManufactDevName_VS (0x1008/0 local OD)
	OplkQtApi::initParam.pHwVersion = NULL;     // NMT_ManufactHwVers_VS  (0x1009/0 local OD)
	OplkQtApi::initParam.pSwVersion = NULL;     // NMT_ManufactSwVers_VS  (0x100A/0 local OD)

//	OplkQtApi::initParam.vendorSpecificExt1 = 0;
//	OplkQtApi::initParam.verifyConfigurationDate = 0; // CFM_VerifyConfiguration_REC.ConfDate_U32
//	OplkQtApi::initParam.verifyConfigurationTime = 0; // CFM_VerifyConfiguration_REC.ConfTime_U32
//	OplkQtApi::initParam.applicationSwDate = 0;       // PDL_LocVerApplSw_REC.ApplSwDate_U32 on programmable device or date portion of NMT_ManufactSwVers_VS on non-programmable device
//	OplkQtApi::initParam.applicationSwTime = 0;       // PDL_LocVerApplSw_REC.ApplSwTime_U32 on programmable device or time portion of NMT_ManufactSwVers_VS on non-programmable device
//	OplkQtApi::initParam.aVendorSpecificExt2[48] = 0;
}

/*******************************************************************************
* Public functions
*******************************************************************************/
tOplkError OplkQtApi::InitStack(const UINT nodeId,
						const std::string& networkInterface)
{
	OplkQtApi::SetInitParam();

	OplkQtApi::initParam.nodeId = nodeId;
	OplkQtApi::initParam.ipAddress = (kIpAddress & kSubnetMask) | OplkQtApi::initParam.nodeId;

	OplkQtApi::initParam.hwParam.pDevName = networkInterface.c_str();

	return (oplk_init(&OplkQtApi::initParam));
}

tOplkError OplkQtApi::StartStack()
{
	tOplkError oplkRet = kErrorGeneralError;

	if (!OplkQtApi::cdcSet)
	{
		qDebug("No custom CDC set, setting default CDC path");
		oplkRet = oplk_setCdcFilename((char*) kDefaultCdcFilename.c_str());
		if (oplkRet != kErrorOk)
		{
			qDebug("Set default CDC File. Ret: %d", oplkRet);
			return oplkRet;
		}
	}

	// Setup processImage according to CiA302_4 profile.
	// TODO: If this is a CN setupPI is not needed.
	oplkRet = oplk_setupProcessImage();
	if (oplkRet != kErrorOk)
	{
		qDebug("SetupProcessImage retCode %x", oplkRet);
		return oplkRet;
	}

	// Starting process thread
	OplkEventHandler::GetInstance().start();

	// Start the OPLK stack by sending NMT s/w reset
	oplkRet = oplk_execNmtCommand(kNmtEventSwReset);
	if (oplkRet != kErrorOk)
		qDebug("kNmtEventSwReset Ret: %d", oplkRet);

#if !defined(CONFIG_KERNELSTACK_DIRECTLINK)
	OplkSyncEventHandler::GetInstance().start();
#endif

	return oplkRet;
}

tOplkError OplkQtApi::StopStack()
{
	tOplkError oplkRet = kErrorGeneralError;

	// cdcSet should be set to false.
	OplkQtApi::cdcSet = false;

	oplkRet = oplk_execNmtCommand(kNmtEventSwitchOff);
	if (oplkRet != kErrorOk)
	{
		qDebug("kNmtEventSwitchOff Ret: %d", oplkRet);
		return oplkRet;
	}

	OplkEventHandler::GetInstance().AwaitNmtGsOff();

	OplkSyncEventHandler::GetInstance().requestInterruption();

	// TODO Set ProcessImage::data to NULL;
	oplkRet = oplk_freeProcessImage();
	if (oplkRet != kErrorOk)
	{
		qDebug("freeProcessImage Ret: %d", oplkRet);
		return oplkRet;
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
		(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection));
}

bool OplkQtApi::UnregisterNodeFoundEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	return QObject::disconnect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalNodeFound),
		&receiver,
		receiverFunction);
}

bool OplkQtApi::RegisterNodeStateChangedEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	/* qRegisterMetaType<T>() is only required for sending the object
	 * through queued signal/slot connections.
	 */
	qRegisterMetaType<tNmtState>("tNmtState");
	return QObject::connect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalNodeStateChanged),
		&receiver,
		receiverFunction,
		(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection));
}

bool OplkQtApi::UnregisterNodeStateChangedEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	return QObject::disconnect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalNodeStateChanged),
		&receiver,
		receiverFunction);
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
		(Qt::ConnectionType)(Qt::QueuedConnection | Qt::UniqueConnection));
}

bool OplkQtApi::UnregisterLocalNodeStateChangedEventHandler(
					const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	return QObject::disconnect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalLocalNodeStateChanged),
		&receiver,
		receiverFunction);
}

bool OplkQtApi::RegisterEventLogger(const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	return QObject::connect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalPrintLog),
		&receiver,
		receiverFunction,
		(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection));
}

bool OplkQtApi::UnregisterEventLogger(const QObject& receiver,
					const QMetaMethod& receiverFunction)
{
	return QObject::disconnect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalPrintLog),
		&receiver,
		receiverFunction);
}

bool OplkQtApi::RegisterSyncEventHandler(Direction::Direction direction,
										const QObject& receiver,
										const QMetaMethod& receiverFunction)
{
	if (direction == Direction::PI_IN)
	{
		return QObject::connect(&OplkSyncEventHandler::GetInstance(),
				QMetaMethod::fromSignal(&OplkSyncEventHandler::SignalUpdateInputValues),
				&receiver,
				receiverFunction,
				(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection));
	}
	else if (direction == Direction::PI_OUT)
	{
		return QObject::connect(&OplkSyncEventHandler::GetInstance(),
				QMetaMethod::fromSignal(&OplkSyncEventHandler::SignalUpdatedOutputValues),
				&receiver,
				receiverFunction,
				(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection));
	}
	// TODO Error exception
	return false;
}

bool OplkQtApi::UnregisterSyncEventHandler(Direction::Direction direction,
											const QObject& receiver,
											const QMetaMethod& receiverFunction)
{
	if (direction == Direction::PI_IN)
	{
		return QObject::disconnect(&OplkSyncEventHandler::GetInstance(),
				QMetaMethod::fromSignal(&OplkSyncEventHandler::SignalUpdateInputValues),
				&receiver,
				receiverFunction);
	}
	else if (direction == Direction::PI_OUT)
	{
		return QObject::disconnect(&OplkSyncEventHandler::GetInstance(),
				QMetaMethod::fromSignal(&OplkSyncEventHandler::SignalUpdatedOutputValues),
				&receiver,
				receiverFunction);
	}
	// TODO Error exception
	return false;
}

tOplkError OplkQtApi::ExecuteNmtCommand(UINT nodeId,
						tNmtCommand nmtCommand)
{
   if (nodeId > 255)
		return kErrorApiInvalidParam;

	// Local node shall use oplk_execNmtCommand(tNmtEvent);
	if (nodeId == 0 || nodeId == OplkQtApi::initParam.nodeId)
	{
		tNmtEvent nmtEvent;
		// Convert tNmtCommand to tNmtEvent.
		switch (nmtCommand)
		{
			case kNmtCmdStopNode:
				nmtEvent = kNmtEventSwitchOff;
				break;

			case kNmtCmdResetNode:
				nmtEvent = kNmtEventResetNode;
				break;

			case kNmtCmdResetCommunication:
				nmtEvent = kNmtEventResetCom;
				break;

			case kNmtCmdResetConfiguration:
				nmtEvent = kNmtEventResetConfig;
				break;

			case kNmtCmdSwReset:
				nmtEvent = kNmtEventSwReset;
				break;

			default:
				return kErrorNmtInvalidParam;
		}
		return oplk_execNmtCommand(nmtEvent);
	}
	else
	{
		tOplkError ret = kErrorOk;
		BOOL releaseCmd = TRUE;

		ret = oplk_writeLocalObject(0x1F9F, 0x03, &nodeId, 0x01);
		if (ret != kErrorOk)
		{
			qDebug("%s() Error: 1F9F/03 : %x\n", __func__, ret);
			return ret;
		}

		ret = oplk_writeLocalObject(0x1F9F, 0x02, &nmtCommand, 0x01);
		if (ret != kErrorOk)
		{
			qDebug("%s() Error: 1F9F/02 : %x\n", __func__, ret);
			return ret;
		}

		// ret = oplk_writeLocalObject(0x1F9F, 0x04, &value, sizeof(value));
		//if (ret != kErrorOk)
		//{
		//  qDebug("%s() Error: 1F9F/04 : %x\n", __func__, ret);
		//   return ret;
		//}

		return oplk_writeLocalObject(0x1F9F, 0x01, &releaseCmd, 0x01);
	}
}

tOplkError OplkQtApi::TransferObject(const SdoTransferJob& sdoTransferJob,
						const QObject& receiver,
						const QMetaMethod& receiverFunction)
{
	ReceiverContext *receiverContext = NULL;

	/* qRegisterMetaType<T>() is only required for sending the object
	 * through queued signal/slot connections.
	 */
	qRegisterMetaType<SdoTransferResult>("SdoTransferResult");

	if ( ((sdoTransferJob.GetNodeId() != initParam.nodeId)
		 && (sdoTransferJob.GetNodeId() != 0)) )
	{
		receiverContext = new ReceiverContext(&receiver, &receiverFunction);
		bool conSuccess = false;
		// Non-Local OD access
		qDebug("Remote OD access, connecting signal.");
		conSuccess = QObject::connect(&OplkEventHandler::GetInstance(),
					QMetaMethod::fromSignal(&OplkEventHandler::SignalSdoTransferFinished),
					&receiver,
					receiverFunction,
					(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection));
		if (!conSuccess)
		{
			qDebug("Connection failed !");
			return kErrorApiInvalidParam;
		}
		qDebug("Connection success");
	}
	else
	{
		//Local OD access
		qDebug("Local OD access, not connecting signal.");
	}

	tSdoComConHdl *sdoComConHdl = new tSdoComConHdl;
	tOplkError oplkRet = kErrorGeneralError;
	UINT dataSize =  sdoTransferJob.GetDataSize();
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
			qDebug("Write Val %lu", ((ULONG)sdoTransferJob.GetData()));
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

	if ((oplkRet != kErrorApiTaskDeferred)
		&& ((sdoTransferJob.GetNodeId() != initParam.nodeId)
		&& (sdoTransferJob.GetNodeId() != 0)))
	{
		// Non-Local OD access: error case

		// Delete the receiver context.
		delete receiverContext;
		delete sdoComConHdl;

		qDebug("Remote OD access, disconnecting signal.");
		bool disconnected = QObject::disconnect(&OplkEventHandler::GetInstance(),
					QMetaMethod::fromSignal(&OplkEventHandler::SignalSdoTransferFinished),
					&receiver,
					receiverFunction);

		if (!disconnected)
		{
			// This is not supposed to happen.
			qDebug("Disconnect failed !");
			return kErrorApiInvalidParam;
		}
		qDebug("Disconnected success");
	}

	return oplkRet;
}

tOplkError OplkQtApi::AllocateProcessImage(ProcessImageIn& in,
						ProcessImageOut& out)
{
	tOplkError oplkRet = kErrorGeneralError;

	/* Allocates the memory for the ProcessImage inside the stack */
	oplkRet = oplk_allocProcessImage(in.GetSize(), out.GetSize());
	if (oplkRet != kErrorOk)
	{
		qDebug("allocProcessImage retCode %x", oplkRet);
		return oplkRet;
	}

	/* sets the ProcessImage pointer from the allocated memory to the ProcessImage::data */
	in.SetProcessImageDataPtr((const BYTE*)oplk_getProcessImageIn());
	out.SetProcessImageDataPtr((const BYTE*)oplk_getProcessImageOut());

	// TODO need to know the use of it. The application was working even before adding it.
	oplkRet = oplk_setupProcessImage();
	if (oplkRet != kErrorOk)
		qDebug("oplk_setupProcessImage retCode %x", oplkRet);
	//defaults to return.

	return oplkRet;
}

tOplkError OplkQtApi::SetCdc(const BYTE* cdcBuffer, const UINT size)
{
	tOplkError oplkRet = oplk_setCdcBuffer((BYTE*) cdcBuffer, size);
	OplkQtApi::cdcSet = (oplkRet == kErrorOk);
	return oplkRet;
}

tOplkError OplkQtApi::SetCdc(const std::string& cdcFileName)
{
	tOplkError oplkRet = oplk_setCdcFilename((char*) cdcFileName.c_str());
	OplkQtApi::cdcSet = (oplkRet == kErrorOk);
	return oplkRet;
}

tOplkError OplkQtApi::SetCycleTime(const ULONG cycleTime)
{
	return (oplk_writeLocalObject(0x1006, 0x00, (void*)&cycleTime, 4));
	// If this is a demo CN. It has to do remote SDO write?.
}

ULONG OplkQtApi::GetSyncWaitTime()
{
	return OplkSyncEventHandler::GetInstance().GetSleepTime();
}

void OplkQtApi::SetSyncWaitTime(const ULONG sleepTime)
{
	OplkSyncEventHandler::GetInstance().SetSleepTime(sleepTime);
}

bool OplkQtApi::RegisterSyncWaitTimeChangedEventHandler(const QObject &receiver,
											const QMetaMethod &receiverFunction)
{
	return QObject::connect(&OplkSyncEventHandler::GetInstance(),
			QMetaMethod::fromSignal(&OplkSyncEventHandler::SignalSyncWaitTimeChanged),
			&receiver,
			receiverFunction,
			(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection));
}

bool OplkQtApi::UnregisterSyncWaitTimeChangedEventHandler(const QObject &receiver,
											const QMetaMethod &receiverFunction)
{
	return QObject::disconnect(&OplkSyncEventHandler::GetInstance(),
			QMetaMethod::fromSignal(&OplkSyncEventHandler::SignalSyncWaitTimeChanged),
			&receiver,
			receiverFunction);
}

bool OplkQtApi::RegisterCriticalErrorEventHandler(const QObject &receiver,
										const QMetaMethod &receiverFunction)
{
	return QObject::connect(&OplkEventHandler::GetInstance(),
			QMetaMethod::fromSignal(&OplkEventHandler::SignalCriticalError),
			&receiver,
			receiverFunction,
			(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection));
}

bool OplkQtApi::UnregisterCriticalErrorEventHandler(const QObject &receiver,
										const QMetaMethod &receiverFunction)
{
	return QObject::disconnect(&OplkEventHandler::GetInstance(),
			QMetaMethod::fromSignal(&OplkEventHandler::SignalCriticalError),
			&receiver,
			receiverFunction);
}


#include "api/OplkQtApi.h"

#define NODEID		0xF0					// MN
#define IP_ADDR		0xc0a86401				// 192.168.100.1
#define SUBNET_MASK	0xFFFFFF00				// 255.255.255.0
#define HOSTNAME	"openPOWERLINK Stack"
#define IF_ETH		EPL_VETH_NAME

#define CYCLE_LEN	5000

CONST BYTE abMacAddr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  /**< Default MAC Address */
static char		networkInterface_g[256];  /**< Network device name */
const char*		sHostname = HOSTNAME;  /**< Default hostname */
static char*	cdcFilename = (char *)"mnobd.cdc";  /**< CDC file name */

tEplApiInitParam OplkQtApi::initParam; /**< initparam */

void OplkQtApi::SetInitParam()
{
	EPL_MEMSET(&initParam, 0, sizeof (initParam));
	initParam.m_uiSizeOfStruct = sizeof (initParam);

	initParam.m_fAsyncOnly = FALSE;

	initParam.m_dwFeatureFlags = UINT_MAX;
	initParam.m_dwCycleLen = CYCLE_LEN;				// required for error detection
	initParam.m_uiIsochrTxMaxPayload = 256;			// const
	initParam.m_uiIsochrRxMaxPayload = 256;			// const
	initParam.m_dwPresMaxLatency = 50000;			// const; only required for IdentRes
	initParam.m_uiPreqActPayloadLimit = 36;			// required for initialisation (+28 bytes)
	initParam.m_uiPresActPayloadLimit = 36;			// required for initialisation of Pres frame (+28 bytes)
	initParam.m_dwAsndMaxLatency = 150000;			// const; only required for IdentRes
	initParam.m_uiMultiplCycleCnt = 0;				// required for error detection
	initParam.m_uiAsyncMtu = 1500;					// required to set up max frame size
	initParam.m_uiPrescaler = 2;					// required for sync
	initParam.m_dwLossOfFrameTolerance = 500000;
	initParam.m_dwAsyncSlotTimeout = 3000000;
	initParam.m_dwWaitSocPreq = 150000;
	initParam.m_dwDeviceType = UINT_MAX;			// NMT_DeviceType_U32
	initParam.m_dwVendorId = UINT_MAX;				// NMT_IdentityObject_REC.VendorId_U32
	initParam.m_dwProductCode = UINT_MAX;			// NMT_IdentityObject_REC.ProductCode_U32
	initParam.m_dwRevisionNumber = UINT_MAX;		// NMT_IdentityObject_REC.RevisionNo_U32
	initParam.m_dwSerialNumber = UINT_MAX;			// NMT_IdentityObject_REC.SerialNo_U32

	initParam.m_dwSubnetMask = SUBNET_MASK;
	initParam.m_dwDefaultGateway = 0;
	EPL_MEMCPY(initParam.m_sHostname, sHostname, sizeof(initParam.m_sHostname));
	initParam.m_uiSyncNodeId = EPL_C_ADR_SYNC_ON_SOA;
	initParam.m_fSyncOnPrcNode = FALSE;

	// write 00:00:00:00:00:00 to MAC address, so that the driver uses the real hardware address
	EPL_MEMCPY(initParam.m_abMacAddress, abMacAddr, sizeof (initParam.m_abMacAddress));

	qDebug("Setting Call back");
	// set callback functions
	initParam.m_pfnCbEvent = OplkEventHandler::GetInstance().GetEventCbFunc();
	if (NULL == initParam.m_pfnCbEvent)
	{
		qDebug("Null Call back");
	}
}

tEplKernel OplkQtApi::InitStack(const UINT nodeId, const std::string& networkInterface)
{
	tEplKernel oplkRet = kEplSuccessful;
	SetInitParam();
	initParam.m_uiNodeId = nodeId;
	initParam.m_dwIpAddress = (IP_ADDR & 0xFFFFFF00) | initParam.m_uiNodeId;

	// Copy the selected interface string to a local variable
	strcpy(networkInterface_g, networkInterface.c_str());
	initParam.m_HwParam.m_pszDevName = networkInterface_g;

	qDebug("init stack");
	oplkRet = oplk_init(&initParam);
	if(oplkRet != kEplSuccessful)
	{
		qDebug("Ret: %d",oplkRet);
		goto Exit;
	}
	//TODO: Replace file pointer by CDC Buffer.
	qDebug("set cdc buffer");
	oplkRet = oplk_setCdcFilename(cdcFilename);
	if(oplkRet != kEplSuccessful)
	{
		qDebug("Ret: %d",oplkRet);
		goto Exit;
	}

Exit:
	 return oplkRet;
}

tEplKernel OplkQtApi::StartStack()
{
	// start process thread
	qDebug("Starting process thread");
	OplkEventHandler::GetInstance().start();

	// Start the OPLK stack by sending NMT reset
	tEplKernel oplkRet = oplk_execNmtCommand(kNmtEventSwReset);
	qDebug("Ret: %d", oplkRet);
	return oplkRet;
}

tEplKernel OplkQtApi::StopStack()
{
	tEplKernel oplkRet;

	oplkRet = oplk_execNmtCommand(kNmtEventSwitchOff);
	OplkEventHandler::GetInstance().AwaitNmtGsOff();
	// OplkEventHandler::GetInstance().terminate();
	oplkRet = oplk_shutdown();
	return oplkRet;
}

bool OplkQtApi::RegisterNodeFoundEventHandler(const QObject& receiver, const QMetaMethod& receiverFunction)
{
	return QObject::connect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalNodeFound),
		&receiver,
		receiverFunction,
		(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection)
	);
}

bool OplkQtApi::UnregisterNodeFoundEventHandler(const QObject& receiver, const QMetaMethod& receiverFunction)
{
	return QObject::connect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalNodeFound),
		&receiver,
		receiverFunction
	);
}

bool OplkQtApi::RegisterNodeStateChangedEventHandler(const QObject& receiver, const QMetaMethod& receiverFunction)
{
	qRegisterMetaType<tNmtState>("tNmtState");
	return QObject::connect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalNodeStateChanged),
		&receiver,
		receiverFunction,
		(Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection)
	);
}

bool OplkQtApi::UnregisterNodeStateChangedEventHandler(const QObject& receiver, const QMetaMethod& receiverFunction)
{
	qRegisterMetaType<tNmtState>("tNmtState");
	return QObject::disconnect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalNodeStateChanged),
		&receiver,
		receiverFunction
	);
}

bool OplkQtApi::RegisterLocalNodeStateChangedEventHandler(const QObject& receiver, const QMetaMethod& receiverFunction)
{
	qRegisterMetaType<tNmtState>("tNmtState");
	return QObject::connect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalLocalNodeStateChanged),
		&receiver,
		receiverFunction,
		(Qt::ConnectionType)(Qt::QueuedConnection | Qt::UniqueConnection)
	);
}

bool OplkQtApi::UnregisterLocalNodeStateChangedEventHandler(const QObject& receiver, const QMetaMethod& receiverFunction)
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

bool OplkQtApi::UnregisterEventLogger(const QObject& receiver, const QMetaMethod& receiverFunction)
{
	return QObject::disconnect(&OplkEventHandler::GetInstance(),
		QMetaMethod::fromSignal(&OplkEventHandler::SignalPrintLog),
		&receiver,
		receiverFunction
	);
}

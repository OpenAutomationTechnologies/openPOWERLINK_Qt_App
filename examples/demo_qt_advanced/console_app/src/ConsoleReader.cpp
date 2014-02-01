/**
********************************************************************************
\file   ConsoleReader.cpp

\brief  Implementation of the console input reader thread.
*******************************************************************************/
#if (TARGET_SYSTEM == _WIN32_)
#define _WINSOCKAPI_
#endif  // (TARGET_SYSTEM == _WIN32_)

#include <pcap.h>

#include "../include/ConsoleReader.h"
#include <iostream>

#ifdef CONFIG_USE_PCAP
extern "C" tEplKernel selectPcapDevice(char *pDevName_p);
#endif // CONFIG_USE_PCAP

ConsoleReader::ConsoleReader()
{
	nodeId = 240;
	devName.reserve(128);
	sdoReadData = new DWORD();
	sdoWriteData = new DWORD();
	*sdoWriteData = 50000;

	sdoReadTransferJob = new SdoTransferJob(1, 0x1006, 0x00, (void*) sdoReadData,
		sizeof(UINT32), kSdoTypeAsnd, kSdoAccessTypeRead);

	sdoWriteTransferJob = new SdoTransferJob(240, 0x1006, 0x00, (void*) sdoWriteData,
		sizeof(UINT32), kSdoTypeAsnd, kSdoAccessTypeWrite);
}

DWORD ConsoleReader::GetSdoReadData()
{
	return *sdoReadData;
}

DWORD ConsoleReader::GetSdoWriteData()
{
	return *sdoWriteData;
}

void ConsoleReader::run()
{
	tEplKernel oplkRet = kEplSuccessful;
	char	cKey = 0;
	BOOL	fExit = FALSE;


#ifdef CONFIG_USE_PCAP
	oplkRet = selectPcapDevice(&devName[0]);
	if (oplkRet != kEplSuccessful)
	{
		qDebug("selectPcapDevice retCode %x", oplkRet);
	}
#else
	devName = "epl";
#endif

	oplkRet = OplkQtApi::InitStack(this->nodeId, devName);
	if (oplkRet != kEplSuccessful)
	{
		qDebug("InitStack retCode %x", oplkRet);
	}

	oplkRet = OplkQtApi::StartStack();
	if (oplkRet != kEplSuccessful)
	{
		qDebug("StartStack retCode %x", oplkRet);
		fExit = TRUE;
	}

	bool stackStated = true;
	while (!fExit)
	{
		if (console_kbhit())
		{
			cKey = (BYTE)console_getch();
			switch (cKey)
			{
				case 'N': //NMT Command
				case 'n':
				{
					oplkRet =  OplkQtApi::ExecuteNmtCommand(1, kNmtCmdResetNode); //tNmtCommand
					if (oplkRet != kEplSuccessful)
					{
					//	fExit = TRUE;
					}
					break;
				}
				case 'R':
				case 'r':
				{
					int methodIndex1 = receiver.metaObject()->indexOfMethod(QMetaObject::normalizedSignature("HandleSdoTransferFinished(const SdoTransferResult)").constData());
					qDebug("Loop Method index1: %d", methodIndex1);
					oplkRet =  OplkQtApi::TransferObject(*sdoReadTransferJob,
							   receiver, receiver.metaObject()->method(methodIndex1));
					qDebug("Ret sdo %x",oplkRet);

					break;
				}
				case 'W':
				case 'w':
				{
					int methodIndex1 = receiver.metaObject()->indexOfMethod(QMetaObject::normalizedSignature("HandleSdoTransferFinished(const SdoTransferResult)").constData());
					qDebug("Loop Method index1: %d", methodIndex1);
					oplkRet =  OplkQtApi::TransferObject(*sdoWriteTransferJob,
							   receiver, receiver.metaObject()->method(methodIndex1));
					qDebug("Ret sdo %x", oplkRet);
					break;
				}
				case 's':
				case 'S':
				{
					if(stackStated)
					{
						oplkRet = OplkQtApi::StopStack();
						if (oplkRet != kEplSuccessful)
						{
							qDebug("StartStack retCode %x", oplkRet);
						}
						stackStated = false;
					}
					else
					{
						oplkRet = OplkQtApi::InitStack(this->nodeId, devName);
						if (oplkRet != kEplSuccessful)
						{
							qDebug("InitStack retCode %x", oplkRet);
						}
						oplkRet = OplkQtApi::StartStack();
						if (oplkRet != kEplSuccessful)
						{
							qDebug("StartStack retCode %x", oplkRet);
						}
						stackStated = true;
					}
					break;
				}
				case 0x1B:
					fExit = TRUE;
					break;

				default:
					break;
			}
			cKey = 0;
		}

		if (oplk_checkKernelStack() == FALSE)
		{
			fExit = TRUE;
			qDebug("Kernel stack has gone! Exiting...\n");
		}
	}
	qDebug("Reader thread Exiting...!\n");
}


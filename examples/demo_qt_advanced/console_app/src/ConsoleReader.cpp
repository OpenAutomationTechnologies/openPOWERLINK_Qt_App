/**
********************************************************************************
\file   ConsoleReader.cpp

\brief  Implementation of the console input reader thread.
*******************************************************************************/

#include "../include/ConsoleReader.h"

ConsoleReader::ConsoleReader()
{
	sdoReadData = new DWORD();
	sdoWriteData = new DWORD();
	*sdoWriteData = 0;

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
	tEplKernel oplkRet;
	char	cKey = 0;
	BOOL	fExit = FALSE;

	while (!fExit)
	{
		if(console_kbhit())
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
}



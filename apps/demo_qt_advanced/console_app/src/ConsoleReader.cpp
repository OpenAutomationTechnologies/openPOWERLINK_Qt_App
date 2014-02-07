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
extern "C" tOplkError selectPcapDevice(char *pDevName_p);
#endif // CONFIG_USE_PCAP

ConsoleReader::ConsoleReader()
{
	nodeId = 240;
	devName.reserve(128);
	xapFileName = "xap.xml";
	sdoReadData = new DWORD();
	sdoWriteData = new DWORD();
	*sdoWriteData = 50000;

	sdoReadTransferJob = new SdoTransferJob(1, 0x1006, 0x00, (void*) sdoReadData,
		sizeof(UINT32), kSdoTypeAsnd, kSdoAccessTypeRead);

	sdoWriteTransferJob = new SdoTransferJob(240, 0x1006, 0x00, (void*) sdoWriteData,
		sizeof(UINT32), kSdoTypeAsnd, kSdoAccessTypeWrite);

	inputChannelName = "CN1.M02.X20DO9322.DigitalOutput01";
	outputChannelName = "CN1.M01.X20DI9371.DigitalInput02";
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
	tOplkError oplkRet = kErrorOk;
	char	cKey = 0;
	BOOL	fExit = FALSE;

	ProcessImageParser *pi =  ProcessImageParser::NewInstance(QT_XML_PARSER);

	std::ifstream ifsXap(xapFileName);
	std::string xapData((std::istreambuf_iterator<char>(ifsXap)), std::istreambuf_iterator<char>());
	pi->Parse(xapData.c_str());

	ProcessImageIn& piIn = static_cast<ProcessImageIn&>(pi->GetProcessImage(PI_IN));
	ProcessImageOut& piOut = static_cast<ProcessImageOut&>(pi->GetProcessImage(PI_OUT));

	qDebug("Size-in: %d", piIn.GetSize());
	qDebug("Size-out: %d", piOut.GetSize());


#ifdef CONFIG_USE_PCAP
	oplkRet = selectPcapDevice(&devName[0]);
	if (oplkRet != kErrorOk)
	{
		qDebug("selectPcapDevice retCode %x", oplkRet);
	}
#else
	devName = "epl";
#endif

	oplkRet = OplkQtApi::InitStack(this->nodeId, devName);
	if (oplkRet != kErrorOk)
	{
		qDebug("InitStack retCode %x", oplkRet);
	}

	oplkRet = OplkQtApi::SetupProcessImage(piIn, piOut);
	if (oplkRet != kErrorOk)
	{
		qDebug("SetupProcessImage retCode %x", oplkRet);
	}

	oplkRet = OplkQtApi::StartStack();
	if (oplkRet != kErrorOk)
	{
		qDebug("StartStack retCode %x", oplkRet);
		fExit = TRUE;
	}

	bool stackStated = true;
	unsigned char *piInDataPtr;
	unsigned char *piOutDataPtr;
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
					if (oplkRet != kErrorOk)
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
				case 't':
				case 'T':
				{
					oplkRet = oplk_exchangeProcessImageOut();
					if (oplkRet != kErrorOk)
					{
						qDebug("ExchangeProcessImageOut retCode %x", oplkRet);
					}
					std::cout<< "\n ProcessImageIn - PReq:  ";
					piInDataPtr = (unsigned char*) oplk_getProcessImageIn();
					for (unsigned int piloop = 0; piloop < piIn.GetSize(); piloop++)
					{
						if (piInDataPtr != NULL)
						{
							std::cout<< std::hex << (int)(*piInDataPtr);
						}
						piInDataPtr++;
					}
					std::cout<< "\n ProcessImageOut - PRes:  ";
					piOutDataPtr = (unsigned char*) oplk_getProcessImageOut();
					for (unsigned int piloop = 0; piloop < piOut.GetSize(); piloop++)
					{
						if (piOutDataPtr != NULL)
						{
							std::cout<< std::hex << (int)(*piOutDataPtr);
						}
						piOutDataPtr++;
					}

					oplkRet = oplk_exchangeProcessImageIn();
					if (oplkRet != kErrorOk)
					{
						qDebug("ExchangeProcessImageIn retCode %x", oplkRet);
					}
					break;
				}
				case 'i':
				case 'I':
				{
					try
					{
						const unsigned char value = 0xFF;
						oplkRet = oplk_exchangeProcessImageOut();
						if (oplkRet != kErrorOk)
						{
							qDebug("ExchangeProcessImageOut retCode %x", oplkRet);
						}
						std::vector<unsigned char> val;
						const Channel channelObj = piIn.GetChannel(inputChannelName);
						val.reserve(channelObj.GetBitSize());
						val.push_back(value);

						//piIn.SetRawValue(inputChannelName, val);
						piIn.SetRawData(val,0,0);

						std::cout<< "\n ProcessImageIn - PReq:  ";
						piInDataPtr = (unsigned char*) oplk_getProcessImageIn();
						for (unsigned int i = 0; i < piIn.GetSize(); i++)
						{
							if (piInDataPtr != NULL)
							{
								std::cout<< std::hex << (int)(*piInDataPtr);
							}
							piInDataPtr++;
						}
						oplkRet = oplk_exchangeProcessImageIn();
						if (oplkRet != kErrorOk)
						{
							qDebug("ExchangeProcessImageIn retCode %x", oplkRet);
						}
					}
					catch(std::out_of_range& ex)
					{
						std::cout << "An Exception has occured: " << ex.what();
					}

					break;
				}
				case 'o':
				case 'O':
				{
					try
					{
						oplkRet = oplk_exchangeProcessImageOut();
						if (oplkRet != kErrorOk)
						{
							qDebug("ExchangeProcessImageOut retCode %x", oplkRet);
						}
						//std::vector<unsigned char> outVal = piOut.GetRawValue(outputChannelName);
						std::vector<unsigned char> outVal = piOut.GetRawData(16,2,0);
						std::cout<< "\nPI-Out Val: ";
						for (std::vector<unsigned char>::const_iterator it = outVal.begin();
								it != outVal.end(); it++)
						{
							//unsigned char ch = *it;
							std::cout << std::hex << (int)(*it);
						}
						std::cout << std::endl;
						oplkRet = oplk_exchangeProcessImageIn();
						if (oplkRet != kErrorOk)
						{
							qDebug("ExchangeProcessImageIn retCode %x", oplkRet);
						}
					}
					catch(std::out_of_range& ex)
					{
						std::cout << "An Exception has occured: " << ex.what();
					}
					break;
				}
				case 's':
				case 'S':
				{
					if(stackStated)
					{
						oplkRet = OplkQtApi::StopStack();
						if (oplkRet != kErrorOk)
						{
							qDebug("StartStack retCode %x", oplkRet);
						}
						stackStated = false;
					}
					else
					{
						oplkRet = OplkQtApi::InitStack(this->nodeId, devName);
						if (oplkRet != kErrorOk)
						{
							qDebug("InitStack retCode %x", oplkRet);
						}
						oplkRet = OplkQtApi::SetupProcessImage(piIn, piOut);
						if (oplkRet != kErrorOk)
						{
							qDebug("SetupProcessImage retCode %x", oplkRet);
						}
						oplkRet = OplkQtApi::StartStack();
						if (oplkRet != kErrorOk)
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


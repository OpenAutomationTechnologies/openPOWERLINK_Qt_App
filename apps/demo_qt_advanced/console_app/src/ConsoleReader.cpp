/**
********************************************************************************
\file   ConsoleReader.cpp

\brief  Implementation of the console input reader thread.
*******************************************************************************/

/*******************************************************************************
* INCLUDES
*******************************************************************************/

#ifdef CONFIG_USE_PCAP
	#if (TARGET_SYSTEM == _WIN32_)
		#define _WINSOCKAPI_
	#endif  // (TARGET_SYSTEM == _WIN32_)
	#include <pcap.h>
#else // CONFIG_USE_PCAP
	#include <oplk/oplkdefs.h>
#endif // CONFIG_USE_PCAP

#include <fstream>
#include <stdexcept>
#include <QDebug>
#include <iostream>
#include "../include/ConsoleReader.h"
#include "console/console.h"
#include "user/processimage/ProcessImageParser.h"
#include "common/XmlParserException.h"

#ifdef CONFIG_USE_PCAP
extern "C" tOplkError selectPcapDevice(char *pDevName_p);
#endif  // CONFIG_USE_PCAP

ConsoleReader::ConsoleReader()
	: nodeId(240),
	  xapFileName("xap.xml"),
	  inputChannelName("CN1.M02.X20DO9322.DigitalOutput01"),
	  outputChannelName("CN1.M02.X20DO9322.NetworkStatus"),
	  sdoWriteData(50000),
	  sdoReceiverFunction("HandleSdoTransferFinished(const SdoTransferResult)")
{
	devName.resize(128);

	sdoReadTransferJob = new SdoTransferJob(0, 0x1006, 0x00, (void*) sdoReadData,
		sizeof(UINT32), kSdoTypeAsnd, kSdoAccessTypeRead);

	sdoWriteTransferJob = new SdoTransferJob(240, 0x1006, 0x00, (void*) sdoWriteData,
		sizeof(UINT32), kSdoTypeAsnd, kSdoAccessTypeWrite);
}

// TODO: warning: type qualifiers ignored on function return type [-Wignored-qualifiers]
// Remove const for std:datatypes
const ULONG ConsoleReader::GetSdoReadData() const
{
	return (this->sdoReadData);
}

const ULONG ConsoleReader::GetSdoWriteData() const
{
	return (this->sdoWriteData);
}

void ConsoleReader::run()
{
	tOplkError oplkRet = kErrorOk;
	char	cKey = 0;
	BOOL	fExit = FALSE;
	ProcessImageParser *pi = NULL;

	try
	{
		pi = ProcessImageParser::NewInstance(ProcessImageParserType::QT_XML_PARSER);
	}
	catch(const std::exception& ex)
	{
		qDebug("An Exception has occurred: %s", ex.what());
	}

	std::ifstream ifsXap(this->xapFileName.c_str());
	std::string xapData((std::istreambuf_iterator<char>(ifsXap)), std::istreambuf_iterator<char>());

	try
	{
		pi->Parse(xapData.c_str());
		// char* a = NULL;
		// pi->Parse(a);
	}
	catch(const XmlParserException& ex)
	{
		qDebug("An Exception has occured: %s", ex.whatException());
	}
	catch(const std::exception& ex)
	{
		qDebug("An Exception has occurred: %s", ex.what());
	}

	ProcessImageIn& piIn = static_cast<ProcessImageIn&>(pi->GetProcessImage(Direction::PI_IN));
	ProcessImageOut& piOut = static_cast<ProcessImageOut&>(pi->GetProcessImage(Direction::PI_OUT));

	qDebug("Size-in: %d", piIn.GetSize());
	qDebug("Size-out: %d", piOut.GetSize());

#ifdef CONFIG_USE_PCAP
	oplkRet = selectPcapDevice(&(this->devName[0]));
	if (oplkRet != kErrorOk)
	{
		qDebug("selectPcapDevice retCode %x", oplkRet);
	}
#else
	devName = EPL_VETH_NAME;
#endif

	oplkRet = OplkQtApi::InitStack(this->nodeId, this->devName);
	if (oplkRet != kErrorOk)
	{
		qDebug("InitStack retCode %x", oplkRet);
	}

	oplkRet = OplkQtApi::AllocateProcessImage(piIn, piOut);
	if (oplkRet != kErrorOk)
	{
		qDebug("AllocateProcessImage retCode %x", oplkRet);
	}

	oplkRet = OplkQtApi::StartStack();
	if (oplkRet != kErrorOk)
	{
		qDebug("StartStack retCode %x", oplkRet);
		fExit = !fExit;
	}

	bool stackStarted = true;
	BYTE *piInDataPtr = NULL;
	BYTE *piOutDataPtr = NULL;
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
					int methodIndex = this->receiver.metaObject()->indexOfMethod(
										QMetaObject::normalizedSignature(sdoReceiverFunction.c_str()).constData());
					if (methodIndex == -1)
					{
						qDebug("SDO Read: SDO Receiver function:%s not found", sdoReceiverFunction.c_str());
					}

					oplkRet =  OplkQtApi::TransferObject(*(this->sdoReadTransferJob),
							   this->receiver, this->receiver.metaObject()->method(methodIndex));

					qDebug("Ret sdo %x",oplkRet);

					break;
				}
				case 'W':
				case 'w':
				{
					int methodIndex = this->receiver.metaObject()->indexOfMethod(
										QMetaObject::normalizedSignature(sdoReceiverFunction.c_str()).constData());
					if (methodIndex == -1)
					{
						qDebug("SDO Write: SDO Receiver function:%s not found", sdoReceiverFunction.c_str());
					}
					oplkRet =  OplkQtApi::TransferObject(*(this->sdoWriteTransferJob),
							   this->receiver, this->receiver.metaObject()->method(methodIndex));
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
					piInDataPtr = (BYTE*) oplk_getProcessImageIn();
					for (UINT piloop = 0; piloop < piIn.GetSize(); piloop++)
					{
						if (piInDataPtr != NULL)
						{
							std::cout<< std::hex << (int)(*piInDataPtr);
						}
						piInDataPtr++;
					}
					std::cout<< "\n ProcessImageOut - PRes:  ";
					piOutDataPtr = (BYTE*) oplk_getProcessImageOut();
					for (UINT piloop = 0; piloop < piOut.GetSize(); piloop++)
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
						oplkRet = oplk_exchangeProcessImageOut();
						if (oplkRet != kErrorOk)
						{
							qDebug("ExchangeProcessImageOut retCode %x", oplkRet);
						}
						const BYTE value = 0xFF;
						std::vector<BYTE> val;
						//const Channel channelObj = piIn.GetChannel(inputChannelName);
						//val.reserve(channelObj.GetBitSize());
						val.push_back(value);

						//piIn.SetRawValue(this->inputChannelName, val);
						piIn.SetRawData(val,0,0);

						std::cout<< "\n ProcessImageIn - PReq:  ";
						piInDataPtr = (BYTE*) oplk_getProcessImageIn();
						for (UINT i = 0; i < piIn.GetSize(); i++)
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
					catch(const std::exception& ex)
					{
						qDebug("An Exception has occurred: %s", ex.what());
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
						//std::vector<BYTE> outVal = piOut.GetRawValue(this->outputChannelName);
						std::vector<BYTE> outVal = piOut.GetRawData(16,2,0);
						std::cout<< "\nPI-Out Val: ";
						for (std::vector<BYTE>::const_iterator it = outVal.begin();
								it != outVal.end(); it++)
						{
							//BYTE ch = *it;
							std::cout << std::hex << (int)(*it);
						}
						std::cout << std::endl;
						oplkRet = oplk_exchangeProcessImageIn();
						if (oplkRet != kErrorOk)
						{
							qDebug("ExchangeProcessImageIn retCode %x", oplkRet);
						}
					}
					catch(const std::exception& ex)
					{
						qDebug("An Exception has occurred: %s", ex.what());
					}
					break;
				}
				case 's':
				case 'S':
				{
					if (stackStarted)
					{
						oplkRet = OplkQtApi::StopStack();
						if (oplkRet != kErrorOk)
						{
							qDebug("StartStack retCode %x", oplkRet);
						}
						stackStarted = false;
					}
					else
					{
						oplkRet = OplkQtApi::InitStack(this->nodeId, this->devName);
						if (oplkRet != kErrorOk)
						{
							qDebug("InitStack retCode %x", oplkRet);
						}
						oplkRet = OplkQtApi::AllocateProcessImage(piIn, piOut);
						if (oplkRet != kErrorOk)
						{
							qDebug("AllocateProcessImage retCode %x", oplkRet);
						}
						oplkRet = OplkQtApi::StartStack();
						if (oplkRet != kErrorOk)
						{
							qDebug("StartStack retCode %x", oplkRet);
						}
						stackStarted = true;
					}
					break;
				}
				case 0x1B:
				{
					fExit = !fExit;

					oplkRet = OplkQtApi::StopStack();
					if (oplkRet != kErrorOk)
					{
						qDebug("StartStack retCode %x", oplkRet);
					}
					stackStarted = false;

					break;
				}

				default:
					break;
			}
			cKey = 0;
		}

		if (oplk_checkKernelStack() == FALSE)
		{
			fExit = !fExit;
			qDebug("Kernel stack has gone! Exiting...\n");
		}
	}
	qDebug("Reader thread Exiting...!\n");
}


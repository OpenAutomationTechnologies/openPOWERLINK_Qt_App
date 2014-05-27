/**
********************************************************************************
\file   ConsoleReader.cpp

\brief  Implementation of the console input reader thread.

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

	sdoReadTransferJob = new SdoTransferJob(3, 0x1006, 0x00, (void*) &sdoReadData,
		sizeof(UINT32), kSdoTypeAsnd, kSdoAccessTypeRead);

	sdoWriteTransferJob = new SdoTransferJob(240, 0x1006, 0x00, (void*) &sdoWriteData,
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

					oplkRet = OplkQtApi::TransferObject(*(this->sdoReadTransferJob),
							   this->receiver, this->receiver.metaObject()->method(methodIndex));

					qDebug("Ret sdo %x", oplkRet);

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
					QDebug print = qDebug();
					print << "\n ProcessImageIn - PReq:  ";
					piInDataPtr = (BYTE*) oplk_getProcessImageIn();
					for (UINT piloop = 0; piloop < piIn.GetSize(); ++piloop)
					{
						if (piInDataPtr != NULL)
						{
							print << (QString("%1").arg(*piInDataPtr, 0, 16)).rightJustified(2, '0');
						}
						++piInDataPtr;
					}
					QDebug print2 = qDebug();
					print2 << "ProcessImageOut - PRes:  ";
					piOutDataPtr = (BYTE*) oplk_getProcessImageOut();
					for (UINT piloop = 0; piloop < piOut.GetSize(); ++piloop)
					{
						if (piOutDataPtr != NULL)
						{
							print2 << (QString("%1").arg(*piOutDataPtr, 0, 16)).rightJustified(2, '0');
						}
						++piOutDataPtr;
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

						QDebug print = qDebug();
						print << "\n ProcessImageIn - PReq:  ";
						piInDataPtr = (BYTE*) oplk_getProcessImageIn();
						for (UINT i = 0; i < piIn.GetSize(); ++i)
						{
							if (piInDataPtr != NULL)
							{
								print << (QString("%1").arg(*piInDataPtr, 0, 16)).rightJustified(2, '0');
							}
							++piInDataPtr;
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
						std::vector<BYTE> outVal = piOut.GetRawValue(this->outputChannelName);
						//std::vector<BYTE> outVal = piOut.GetRawData(16,2,0);
						QDebug print = qDebug();
						print << "\n PI-Out Val: ";
						for (std::vector<BYTE>::const_iterator it = outVal.begin();
								it != outVal.end(); ++it)
						{
							print << (QString("%1").arg(*it, 0, 16)).rightJustified(2, '0');
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
						qDebug("Stop Stack retCode %x", oplkRet);
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


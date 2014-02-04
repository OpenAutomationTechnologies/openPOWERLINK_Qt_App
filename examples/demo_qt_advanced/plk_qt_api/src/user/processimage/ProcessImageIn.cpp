/**
********************************************************************************
\file   ProcessImageIn.cpp

\brief  Implementation of ProcessImageIn class
*******************************************************************************/

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

#include "user/processimage/ProcessImageIn.h"
#include <bitset>

ProcessImageIn::ProcessImageIn()
{

}

ProcessImageIn::ProcessImageIn(const unsigned int byteSize,
		const std::map<std::string, Channel>& channels)
		: ProcessImage(byteSize, channels)
{

}

bool ProcessImageIn::AddChannelInternal(const Channel& channel)
{
	bool retVal = false;
	if (channel.GetDirection() == PI_IN)
	{
		this->channels.insert(std::pair<std::string, Channel>(channel.GetName(),
															  channel));
		retVal = true;
	}

	return retVal;
}

void ProcessImageIn::SetRawValue(const std::string& channelName,
						std::vector<unsigned char>& value)
{
	Channel channelObj = this->GetChannel(channelName);
	if (channelObj.GetBitSize() == value.size())
	{
		this->SetRawData(value, channelObj.GetByteOffset(),
				channelObj.GetBitOffset());
	}
	else
	{
		qDebug("vector size and channel size did'nt match");
	}
}

void ProcessImageIn::SetRawData(const std::vector<unsigned char>& value,
						const unsigned int byteOffset,
						const unsigned int bitOffset)
{
	unsigned char* piDataPtr = this->GetProcessImageDataPtr();
	if (piDataPtr)
	{
		const unsigned int bitSize = this->GetChannelsBitSize(byteOffset, bitOffset);
		piDataPtr += byteOffset;
		if ((bitSize % 8) == 0)
		{
			// Bitsize will be multiples of 8 except for BITSTRING.
			for (unsigned int i = 0; i < (bitSize/8); i++)
			{
				qDebug(" Start ::%d  ::%d", *piDataPtr, value[i]);
				piDataPtr += i;
				*piDataPtr = value[i];
			}
		}
		else
		{
//			if (bitSize == 1)
//			{
//				std::bitset<8> piData = *piDataPtr;
//				std::bitset<8> bitValue = value[0];
//				piData.set(bitOffset, bitValue[0]);
//				*piDataPtr = piData;
//			}
//			else
			if (bitSize < 8)
			{
				std::bitset<8> piData = *piDataPtr;
				std::bitset<8> bitValue = value[0];
				for (unsigned int i = 0; i < bitSize; i++)
				{
					piData.set((bitOffset + i), bitValue[i]);
				}
				unsigned long longVal = piData.to_ulong();
				unsigned char piDataNew = static_cast<unsigned char>(longVal);
				*piDataPtr = piDataNew;
				std::cout << std::hex << (int)piDataNew;
			}
			else
			{
				//unhandled datatype
			}
		}
	}
	else
	{
		//piDataPtr not set.
	}
}

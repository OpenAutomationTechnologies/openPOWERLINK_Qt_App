/**
********************************************************************************
\file   ProcessImageOut.cpp

\brief  Defines the methods that are specific to the output process image.

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

#include "user/processimage/ProcessImageOut.h"
#include <bitset>

ProcessImageOut::ProcessImageOut()
{
}

ProcessImageOut::ProcessImageOut(const UINT byteSize,
					const std::map<std::string, Channel>& channels)
					: ProcessImage(byteSize, channels)
{

}

bool ProcessImageOut::AddChannelInternal(const Channel& channel)
{
	bool retVal = false;
	if (channel.GetDirection() == Direction::PI_OUT)
	{
		this->channels.insert(std::pair<std::string, Channel>(channel.GetName(),
															 channel));
		retVal = true;
	}
	return retVal;
}

std::vector<BYTE> ProcessImageOut::GetRawValue(const std::string& channelName) const
{
	Channel channelObj = this->GetChannel(channelName);
	return this->GetRawData(channelObj.GetBitSize(),
					channelObj.GetByteOffset(),
					channelObj.GetBitOffset());
}

std::vector<BYTE> ProcessImageOut::GetRawData(const UINT bitSize,
											const UINT byteOffset,
											const UINT bitOffset) const
{
	std::vector<BYTE> rawData;
	BYTE* piDataPtr = this->GetProcessImageDataPtr();
	if (piDataPtr)
	{
		piDataPtr += byteOffset;
		if ((bitSize % 8) == 0)
		{
			rawData.reserve(bitSize/8);
			for (UINT i = 0; i < (bitSize/8); i++)
			{
				 piDataPtr += i;
				 rawData.push_back(*piDataPtr);
			}
		}
		else
		{
			if (bitSize < 8)
			{
				rawData.reserve(1);
				std::bitset<8> piData = *piDataPtr;
				std::bitset<8> bitValue;
				for (UINT cntr = 0; cntr < bitSize; cntr++)
				{
					bitValue.set(cntr, piData[bitOffset + cntr]);
				}
				ULONG longVal = bitValue.to_ulong();
				BYTE data = static_cast<BYTE>(longVal);
				rawData.push_back(data);
			}
			else
			{
				//unhandled datatype
				rawData.reserve(0);
			}
		}
	}
	else
	{
		// piDataPtr not set.
		rawData.reserve(0);
	}
	return rawData;
}


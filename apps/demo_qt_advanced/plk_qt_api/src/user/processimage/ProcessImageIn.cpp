/**
********************************************************************************
\file   ProcessImageIn.cpp

\brief  Defines the methods that are specific to the input process image.

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
#include "user/processimage/ProcessImageIn.h"
#include <bitset>
#include <sstream>
#include <stdexcept>

/*******************************************************************************
* Public functions
*******************************************************************************/

ProcessImageIn::ProcessImageIn() : ProcessImage()
{

}

ProcessImageIn::ProcessImageIn(const UINT byteSize,
		const std::map<std::string, Channel>& channels)
		: ProcessImage(byteSize, channels)
{

}

bool ProcessImageIn::AddChannelInternal(const Channel& channel)
{
	if (channel.GetDirection() == Direction::PI_IN)
	{
		this->channels.insert(
				std::pair<std::string, Channel>(channel.GetName(), channel));
		return true;
	}
	return false;
}

void ProcessImageIn::SetRawValue(const std::string& channelName,
						std::vector<BYTE>& value)
{
	Channel channel = this->GetChannel(channelName);
	BYTE* piDataPtr = this->GetProcessImageDataPtr();
	if (piDataPtr)
	{
		// Move the PI data pointer to the given channels byte offset.
		piDataPtr += channel.GetByteOffset();

		const UINT bitSize = channel.GetBitSize();
		if ((bitSize % 8) == 0)
		{
			for (UINT i = 0; i < (bitSize / 8); ++i)
			{
				piDataPtr += i;
				*piDataPtr = value[i];
			}
		}
		else if (bitSize < 8)
		{
			std::bitset<8> piFirstByte = *piDataPtr;
			std::bitset<8> extractedFirstByte = value[0];
			for (UINT pos = 0; pos < bitSize; ++pos)
			{
				piFirstByte.set((channel.GetBitOffset() + pos), extractedFirstByte[pos]);
			}
			ULONG longVal = piFirstByte.to_ulong();
			*piDataPtr =  static_cast<BYTE>(longVal);
		}
		else
		{
			// TODO Discuss. Bitsize is multiples of 8. or ranges from 0-7.
			std::ostringstream msg;
			msg << "Invalid bitSize for the channel:" << channelName;
			msg << ". bitSize: " << bitSize ;
			throw std::invalid_argument(msg.str());
		}
	}
	else
	{
		//TODO Discuss. Fails in Linux
//		std::ostringstream msg;
//		msg << " ProcessImage data pointer is NULL.";
//		msg << "Check the allocation of memory for the input ProcessImage";
//		throw std::bad_alloc(msg.str().c_str());
	}
}

void ProcessImageIn::SetRawData(const std::vector<BYTE>& value,
						const UINT byteOffset,
						const UINT bitOffset)
{
// TODO: Check for Powerlink possible maximum for input args.
	if ((byteOffset + value.size()) > this->GetSize())
	{
		std::ostringstream msg;
		msg << "The size of the value+byteOffset:" << (value.size() + byteOffset);
		msg << " exceeds the size of the ProcessImage:" << this->GetSize() << " bytes";
		throw std::out_of_range(msg.str());
	}

	BYTE* piDataPtr = this->GetProcessImageDataPtr();
	if (piDataPtr)
	{
		/* Move the data pointer to the byteOffsets position */
		piDataPtr += byteOffset;
		if (bitOffset == 0)
		{
			for (UINT i = 0; i < value.size(); ++i)
			{
				piDataPtr += i;
				*piDataPtr = value[i];
			}
		}
		else if ((bitOffset < 8))
		{
			/* Update 1st byte based on the biOffset */
			std::bitset<8> piFirstByte = *piDataPtr;
			std::bitset<8> extractedFirstByte = value[0];

			for (UINT pos = bitOffset; pos < 8; ++pos)
			{
				piFirstByte.set(pos, extractedFirstByte[pos]);
			}
			ULONG longVal = piFirstByte.to_ulong();

			/* set the updated value to the piData pointer. */
			*piDataPtr = static_cast<BYTE>(longVal);

			/* Update the data, start from the 2nd byte if available */
			for (UINT i = 1; i < value.size(); ++i)
			{
				piDataPtr += i;
				*piDataPtr = value[i];
			}
		}
		else
		{
			std::ostringstream msg;
			msg << "Invalid BitOffset. Exceeds its range(0x00 to 0x07). ";
			msg << "bitOffset : " << bitOffset ;
			throw std::invalid_argument(msg.str());
		}
	}
	else
	{
		//TODO Discuss Fails in Linux
//		std::ostringstream msg;
//		msg << " ProcessImage data pointer is NULL.";
//		msg << "Check the allocation of memory for the input ProcessImage";
//		throw std::bad_alloc(msg.str().c_str());
	}
}

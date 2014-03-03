/**
********************************************************************************
\file   ProcessImage.cpp

\brief  Implementation of the ProcessImage class which provides sophisticated
		methods to access the different attributes of a ProcessImage in the xap.xml.

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

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <sstream>
#include <stdexcept>

#include "user/processimage/ProcessImage.h"

/*******************************************************************************
* Public functions
*******************************************************************************/

ProcessImage::ProcessImage() :
				byteSize(0),
				channels(),
				data(NULL)

{

}

ProcessImage::ProcessImage(const UINT byteSize,
		const std::map<std::string, Channel>& channels) :
		byteSize(byteSize),
		data(NULL)
{
	for (std::map<std::string, Channel>::const_iterator cIt = channels.begin();
		 cIt != channels.end(); ++cIt)
	{
		bool res = this->AddChannel(cIt->second);
		// TODO: return result of add channel.
	}
}

//TODO: C4711 Selected for automatic inline expression.
ProcessImage::~ProcessImage()
{
}

void ProcessImage::SetSize(UINT byteSize)
{
	this->byteSize = byteSize;
}

UINT ProcessImage::GetSize() const
{
	return this->byteSize;
}

void ProcessImage::SetProcessImageDataPtr(const BYTE* data)
{
	this->data = (BYTE*) data;
}

BYTE* ProcessImage::GetProcessImageDataPtr() const
{
	return this->data;
}

std::map<std::string, Channel>::const_iterator ProcessImage::cbegin() const
{
	// C++11
	//TODO Fails in Linux
	return this->channels.cbegin();
}

std::map<std::string, Channel>::const_iterator ProcessImage::cend() const
{
	// C++11
	//TODO Fails in Linux
	return this->channels.cend();
}

const Channel ProcessImage::GetChannel(const std::string& name) const
{
	std::map<std::string, Channel>::const_iterator cIt = channels.find(name);
	if (cIt != channels.end())
	{
		return cIt->second;
	}
	else
	{
		std::ostringstream message;
		message << "Requested channel '" << name << "' not found in process image.";
		throw std::out_of_range(message.str());
	}
}

const std::vector<Channel> ProcessImage::GetChannelsByOffset(const UINT byteOffset) const
{
	std::vector<Channel> channelCollection;

	for (std::map<std::string, Channel>::const_iterator cIt = channels.begin();
		 cIt != channels.end(); ++cIt)
	{
		if (byteOffset == cIt->second.GetByteOffset())
		{
			channelCollection.push_back(cIt->second);
		}
//		std::cout << cIt->first << " => " << cIt->second.GetByteOffset() << '\n';
	}
	return channelCollection;
}

const std::vector<Channel> ProcessImage::GetChannelsByNodeId(const UINT nodeId) const
{
	std::vector<Channel> channelCollection;

	std::ostringstream nodeName("CN");
	nodeName << nodeId;

	for (std::map<std::string, Channel>::const_iterator cIt = channels.begin();
		 cIt != channels.end(); ++cIt)
	{
		std::string channelName = cIt->first;
		UINT pos = channelName.find(".");
		std::string extractedNodeName = channelName.substr(0, pos);

		if (extractedNodeName == nodeName.str())
		{
			channelCollection.push_back(cIt->second);
		}
	}
	return channelCollection;
}

bool ProcessImage::AddChannel(const Channel& channel)
{
	return this->AddChannelInternal(channel);
}

void ProcessImage::ResetProcessImage()
{
	this->channels.clear();
	this->byteSize = 0;
	this->data = NULL;
}

template <class T>
T ProcessImage::GetValue(const std::string& channelName) const
{
	// TODO
	return 0;
}

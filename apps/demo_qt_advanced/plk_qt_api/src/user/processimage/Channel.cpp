/**
********************************************************************************
\file   Channel.cpp

\brief  Implementation of the channel class which provides methods
		to access the different attributes of a channel in a ProcessImage

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

#include "user/processimage/Channel.h"

Channel::Channel(const std::string& name,
		IECDataType::eIECDataType dataType,
		const UINT byteOffset,
		const UINT bitOffset,
		const UINT bitSize,
		Direction::eDirection direction) :
		name(name),
		dataType(dataType),
		byteOffset(byteOffset),
		bitOffset(bitOffset),
		bitSize(bitSize),
		direction(direction)
{

}

const std::string& Channel::GetName() const
{
	return this->name;
}

IECDataType::eIECDataType Channel::GetDataType() const
{
	return this->dataType;
}

UINT Channel::GetByteOffset() const
{
	return this->byteOffset;
}

UINT Channel::GetBitOffset() const
{
	return this->bitOffset;
}

UINT Channel::GetBitSize() const
{
	return this->bitSize;
}

Direction::eDirection Channel::GetDirection() const
{
	return this->direction;
}


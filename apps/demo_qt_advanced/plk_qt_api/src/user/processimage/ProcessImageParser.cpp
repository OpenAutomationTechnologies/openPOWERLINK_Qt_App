/**
********************************************************************************
\file   ProcessImageParser.cpp

\brief  Implements the process image parser abstract class.

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

#include "user/processimage/ProcessImageParser.h"
#include "user/processimage/QtProcessImageParser.h"

const std::string ProcessImageParser::piType = "type";
const std::string ProcessImageParser::byteSize = "size";

const std::string ProcessImageParser::name = "Name";
const std::string ProcessImageParser::dataType = "dataType";
const std::string ProcessImageParser::bitSize = "dataSize";
const std::string ProcessImageParser::byteOffset = "PIOffset";
const std::string ProcessImageParser::bitOffset = "BitOffset";

const std::string ProcessImageParser::tagApplicationProcess = "ApplicationProcess";
const std::string ProcessImageParser::tagProcessImage = "ProcessImage";
const std::string ProcessImageParser::tagChannel = "Channel";

ProcessImageParser::ProcessImageParser()
{
}

ProcessImageParser::~ProcessImageParser()
{

}

ProcessImageParser* ProcessImageParser::NewInstance(const ProcessImageParserType type)
{
	// Create and return an instance of a ProcessImageParser according to the given type.
	if (type == QT_XML_PARSER)
	{
		static QtProcessImageParser qtPiParser;
		return &qtPiParser;
	}
	else
	{
		return NULL;
	}
}

ProcessImage& ProcessImageParser::GetProcessImage(const Direction direction)
{
	if (direction == PI_IN)
	{
		return in;
	}
	else
	{
		return out;
	}
}

void ProcessImageParser::Parse(const char* description)
{
	this->ParseInternal(description);
}

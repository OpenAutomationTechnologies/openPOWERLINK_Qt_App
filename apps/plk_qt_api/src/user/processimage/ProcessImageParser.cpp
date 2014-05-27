/**
********************************************************************************
\file   ProcessImageParser.cpp

\brief  Implements the process image parser abstract class.

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
#include <sstream>

#include "user/processimage/ProcessImageParser.h"
#include "user/processimage/QtProcessImageParser.h"

/*******************************************************************************
* Static member variables
*******************************************************************************/
const std::string ProcessImageParser::processImage_attribute_Type = "type";
const std::string ProcessImageParser::processImage_attribute_byteSize = "size";

const std::string ProcessImageParser::processImage_Type_input = "input";
const std::string ProcessImageParser::processImage_Type_output = "output";

const std::string ProcessImageParser::channel_attribute_name = "Name";
const std::string ProcessImageParser::channel_attribute_dataType = "dataType";
const std::string ProcessImageParser::channel_attribute_bitSize = "dataSize";
const std::string ProcessImageParser::channel_attribute_byteOffset = "PIOffset";
const std::string ProcessImageParser::channel_attribute_bitOffset = "BitOffset";
const std::string ProcessImageParser::applicationProcess_element_name = "ApplicationProcess";
const std::string ProcessImageParser::processImage_element_name = "ProcessImage";
const std::string ProcessImageParser::channel_element_name = "Channel";

const std::string ProcessImageParser::processImage_attribute_type_not_found =
		ProcessImageParser::processImage_element_name + "attribute '" +
		ProcessImageParser::processImage_attribute_Type + "' not found";

const std::string ProcessImageParser::processImage_attribute_byteSize_not_found =
		ProcessImageParser::processImage_element_name + "attribute '" +
		ProcessImageParser::processImage_attribute_byteSize + "' not found";

const std::string ProcessImageParser::channel_attribute_name_not_found =
		ProcessImageParser::channel_element_name + "attribute '" +
		ProcessImageParser::channel_attribute_name + "' not found";

const std::string ProcessImageParser::channel_attribute_dataType_not_found =
		ProcessImageParser::channel_element_name + "attribute '" +
		ProcessImageParser::channel_attribute_dataType + "' not found";

const std::string ProcessImageParser::channel_attribute_bitSize_not_found =
		ProcessImageParser::channel_element_name + "attribute '" +
		ProcessImageParser::channel_attribute_bitSize + "' not found";

const std::string ProcessImageParser::channel_attribute_byteOffset_not_found =
		ProcessImageParser::channel_element_name + "attribute '" +
		ProcessImageParser::channel_attribute_byteOffset + "' not found";

const std::string ProcessImageParser::channel_attribute_bitOffset_not_found =
		ProcessImageParser::channel_element_name + "attribute '" +
		ProcessImageParser::channel_attribute_bitOffset + "' not found";

const std::string ProcessImageParser::processImage_attribute_type_invalid_value =
		ProcessImageParser::processImage_element_name + "attribute '" +
		ProcessImageParser::processImage_attribute_Type + "' has invalid value";

/*******************************************************************************
* Public functions
*******************************************************************************/
ProcessImageParser::~ProcessImageParser()
{

}

ProcessImageParser* ProcessImageParser::NewInstance(
		const ProcessImageParserType::ProcessImageParserType type)
{
	if (type == ProcessImageParserType::QT_XML_PARSER)
	{
		return (new QtProcessImageParser());
	}
	else
	{
		std::ostringstream message;
		message << "The requested ProcessImageParser is not available";
		throw std::invalid_argument(message.str());
	}
}

ProcessImage& ProcessImageParser::GetProcessImage(const Direction::Direction direction)
{
	if (direction == Direction::PI_IN)
	{
		return this->in;
	}
	else
	{
		return this->out;
	}
}

void ProcessImageParser::Parse(const char* xmlDescription)
{
	this->ParseInternal(xmlDescription);
}

/*******************************************************************************
* Protected functions
*******************************************************************************/
ProcessImageParser::ProcessImageParser()
{
}

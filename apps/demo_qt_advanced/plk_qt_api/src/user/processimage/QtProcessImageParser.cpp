/**
********************************************************************************
\file   QtProcessImageParser.cpp

\brief  Implementation of a process image parser that utilizes a Qt 5.2
		QXmlStreamReader to parse the process image description

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

#include "user/processimage/QtProcessImageParser.h"

void QtProcessImageParser::ParseInternal(const char* description)
{

	this->xml.addData(description);

	while(!this->xml.atEnd() && !this->xml.hasError())
	{
		QXmlStreamReader::TokenType token = this->xml.readNext();
		/* If token is StartDocument, Go to next.*/
		if (token == QXmlStreamReader::StartDocument)
		{
			continue;
		}
		/* If token is StartElement, See if we can read it.*/
		if (token == QXmlStreamReader::StartElement)
		{
			/* If it's named ApplicationProcess, Go to the next.*/
			if (this->xml.name() == QString::fromStdString(ProcessImageParser::applicationProcess_element_name).right(-1))
			{
				continue;
			}
			if (this->xml.name() == QString::fromStdString(ProcessImageParser::processImage_element_name).right(-1))
			{
				/* Let's check that we're really getting a ProcessImage. */
				if (this->xml.tokenType() != QXmlStreamReader::StartElement &&
						this->xml.name() == QString::fromStdString(ProcessImageParser::processImage_element_name).right(-1))
				{
					continue;
				}
				this->ParseProcessImage();
			}
		}
	}
	if (this->xml.hasError()) {
		//Throw err?
	}
	this->xml.clear();
}

void QtProcessImageParser::ParseProcessImage()
{
	while (!(this->xml.tokenType() == QXmlStreamReader::EndElement
			 && this->xml.name() == QString::fromStdString(ProcessImageParser::applicationProcess_element_name).right(-1)))
	{
		if (this->xml.tokenType() == QXmlStreamReader::StartElement)
		{
			/* Get the attributes for ProcessImage */
			QXmlStreamAttributes attrib = this->xml.attributes();
			Direction::eDirection direction = Direction::UNDEFINED;
			unsigned int byteSize = 0;
			if (attrib.hasAttribute(QString::fromStdString(ProcessImageParser::processImage_attribute_Type)))
			{
				QString piType;
				piType =  attrib.value(QString::fromStdString(ProcessImageParser::processImage_attribute_Type)).toString();
				direction = (piType == QString::fromStdString(ProcessImageParser::processImage_Type_output)) ? Direction::PI_OUT :
							(piType == QString::fromStdString(ProcessImageParser::processImage_Type_input)) ? Direction::PI_IN : direction;
			}

			if (attrib.hasAttribute(QString::fromStdString(ProcessImageParser::processImage_attribute_byteSize)))
			{
				QString size_str = attrib.value(QString::fromStdString(ProcessImageParser::processImage_attribute_byteSize)).toString();
				byteSize = size_str.toUInt();
			}

			//optional Fix for openCONFIGURATOR - ProcessImage total size.
			if ((byteSize%4) != 0 )
			{
				byteSize = ((byteSize/4)+1)*4;
			}

			if (direction == Direction::PI_OUT)
			{
				this->out.SetSize(byteSize);
			}
			else if (direction == Direction::PI_IN)
			{
				this->in.SetSize(byteSize);
			}
			else
			{
				//Throw error?
			}
			/* Next element... */
			this->xml.readNext();

			this->ParseChannels(direction);
		}
		this->xml.readNext();
	}
}

void QtProcessImageParser::ParseChannels(Direction::eDirection direction)
{
	/*
	 * Loop over the things because the order might change.
	 * Continue the loop until we hit an EndElement named ProcessImage.
	 */
	std::string name = "";
	IECDataType::eIECDataType dataType = IECDataType::UNDEFINED;
	unsigned int byteOffset = 0;
	unsigned int bitOffset = 0;
	unsigned int bitSize = 0;

	while (!(this->xml.tokenType() == QXmlStreamReader::EndElement
			 && this->xml.name() == QString::fromStdString(ProcessImageParser::processImage_element_name).right(-1)))
	{
		if (this->xml.tokenType() == QXmlStreamReader::StartElement)
		{
			if (this->xml.name() == QString::fromStdString(ProcessImageParser::channel_element_name).right(-1))
			{
				if ((this->xml.tokenType() != QXmlStreamReader::StartElement)
						&& (this->xml.name() == QString::fromStdString(ProcessImageParser::channel_element_name).right(-1)))
				{
					return;
				}

				/* Get the attributes for Channel */
				QXmlStreamAttributes attributes = this->xml.attributes();

				if (attributes.hasAttribute(QString::fromStdString(ProcessImageParser::channel_attribute_name)))
				{
					name = attributes.value(QString::fromStdString(ProcessImageParser::channel_attribute_name)).toString().toStdString();
				}
				if (attributes.hasAttribute(QString::fromStdString(ProcessImageParser::channel_attribute_dataType)))
				{
					std::string iecDataTypeStr;
					iecDataTypeStr = attributes.value(QString::fromStdString(ProcessImageParser::channel_attribute_dataType)).toString().toStdString();
					dataType = IECDataType::GetIECDatatype(iecDataTypeStr);
				}

				if (attributes.hasAttribute(QString::fromStdString(ProcessImageParser::channel_attribute_bitSize)))
				{
					bitSize = attributes.value(QString::fromStdString(ProcessImageParser::channel_attribute_bitSize)).toString().toUInt();
				}

				if (attributes.hasAttribute(QString::fromStdString(ProcessImageParser::channel_attribute_byteOffset)))
				{
					byteOffset = attributes.value(QString::fromStdString(ProcessImageParser::channel_attribute_byteOffset)).toString().toUInt(0, 16);
				}

				if (attributes.hasAttribute(QString::fromStdString(ProcessImageParser::channel_attribute_bitOffset)))
				{
					bitOffset = attributes.value(QString::fromStdString(ProcessImageParser::channel_attribute_bitOffset)).toString().toUInt(0, 16);
				}

				Channel chObj(name, dataType, byteOffset, bitOffset, bitSize, direction);
				//std::cout << "Channel" << name << " : " << dataType <<" : " << byteOffset <<" : " << bitOffset <<" : " << bitSize <<" : " << direction << std::endl;
				if (direction == Direction::PI_IN)
				{
					this->in.AddChannel(chObj);
				}
				else if (direction == Direction::PI_OUT)
				{
					this->out.AddChannel(chObj);
				}
				else
				{
					// Not possible
					//Throw err?
				}
			}
		}
		/* ...and next... */
		this->xml.readNext();
	}
}


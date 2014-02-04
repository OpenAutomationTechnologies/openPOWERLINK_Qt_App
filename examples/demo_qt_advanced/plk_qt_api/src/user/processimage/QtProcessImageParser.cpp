/**
********************************************************************************
\file   QtProcessImageParser.cpp

\brief  Implementation of QtProcessImageParser class
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

#include "user/processimage/QtProcessImageParser.h"

void QtProcessImageParser::ParseInternal(const char* description)
{

	xml.addData(description);

	while(!xml.atEnd() && !xml.hasError())
	{
		QXmlStreamReader::TokenType token = xml.readNext();
		/* If token is StartDocument, Go to next.*/
		if (token == QXmlStreamReader::StartDocument)
		{
			continue;
		}
		/* If token is StartElement, See if we can read it.*/
		if (token == QXmlStreamReader::StartElement)
		{
			/* If it's named ApplicationProcess, Go to the next.*/
			if (xml.name() == "ApplicationProcess")
			{
				continue;
			}
			if (xml.name() == "ProcessImage")
			{
				/* Let's check that we're really getting a ProcessImage. */
				if (xml.tokenType() != QXmlStreamReader::StartElement &&
						xml.name() == "ProcessImage")
				{
					continue;
				}
				this->ParseProcessImage();
			}
		}
	}
	if (xml.hasError()) {
		//Throw err?
	}
	xml.clear();
}

void QtProcessImageParser::ParseProcessImage()
{
	while (!(xml.tokenType() == QXmlStreamReader::EndElement
			 && xml.name() == "ApplicationProcess"))
	{
		if (xml.tokenType() == QXmlStreamReader::StartElement)
		{
			/* Get the attributes for ProcessImage */
			QXmlStreamAttributes attrib = xml.attributes();
			Direction direction = static_cast<Direction>(-1);
			unsigned int byteSize = 0;
			if (attrib.hasAttribute(QString::fromStdString(this->piType)))
			{
				QString piType;
				piType =  attrib.value(QString::fromStdString(this->piType)).toString();
				direction = (piType == "output") ? PI_OUT :
							(piType == "input") ? PI_IN : static_cast<Direction>(-1);
			}

			if (attrib.hasAttribute(QString::fromStdString(this->byteSize)))
			{
				QString size_str = attrib.value(QString::fromStdString(this->byteSize)).toString();
				byteSize = size_str.toUInt();
			}

			if (direction == PI_OUT)
			{
				this->out.SetSize(byteSize);
			}
			else if (direction == PI_IN)
			{
				this->in.SetSize(byteSize);
			}
			else
			{
				//Throw error?
			}
			/* Next element... */
			xml.readNext();

			this->ParseChannels(direction);
		}
		xml.readNext();
	}
}

void QtProcessImageParser::ParseChannels(Direction direction)
{
	/*
	 * Loop over the things because the order might change.
	 * Continue the loop until we hit an EndElement named ProcessImage.
	 */
	std::string name = "";									//Name="CN1.X20DO9322.DigitalOutput01"
	IECDataType dataType = static_cast<IECDataType>(-1);	//dataType="BITSTRING" from IECDatatype
	unsigned int byteOffset = 0;							//PIOffset="0x0000"
	unsigned int bitOffset = 0;								//BitOffset="0x00"
	unsigned int bitSize = 0;								//dataSize="1"

	while (!(xml.tokenType() == QXmlStreamReader::EndElement
			 && xml.name() == "ProcessImage"))
	{
		if (xml.tokenType() == QXmlStreamReader::StartElement)
		{
			if (xml.name() == "Channel")
			{
				if ((xml.tokenType() != QXmlStreamReader::StartElement)
						&& (xml.name() == "Channel"))
				{
					return;
				}

				/* Get the attributes for Channel */
				QXmlStreamAttributes attributes = xml.attributes();

				if (attributes.hasAttribute(QString::fromStdString(this->name)))
				{
					name = attributes.value(QString::fromStdString(this->name)).toString().toStdString();
				}
				if (attributes.hasAttribute(QString::fromStdString(this->dataType)))
				{
					std::string iecDataTypeStr;
					iecDataTypeStr = attributes.value(QString::fromStdString(this->dataType)).toString().toStdString();
					if (!GetIECDatatype(iecDataTypeStr, dataType))
					{
						//std::cout << iecDataTypeStr;
						dataType = static_cast<IECDataType>(-1);
					}
				}

				if (attributes.hasAttribute(QString::fromStdString(this->bitSize)))
				{
					bitSize = attributes.value(QString::fromStdString(this->bitSize)).toString().toUInt();
				}

				if (attributes.hasAttribute(QString::fromStdString(this->byteOffset)))
				{
					byteOffset = attributes.value(QString::fromStdString(this->byteOffset)).toString().toUInt(0, 16);
				}

				if (attributes.hasAttribute(QString::fromStdString(this->bitOffset)))
				{
					bitOffset = attributes.value(QString::fromStdString(this->bitOffset)).toString().toUInt(0, 16);
				}

				Channel chObj(name, dataType, byteOffset, bitOffset, bitSize, direction);
				//std::cout << "Channel" << name << " : " << dataType <<" : " << byteOffset <<" : " << bitOffset <<" : " << bitSize <<" : " << direction << std::endl;
				if (direction == PI_IN)
				{
					this->in.AddChannel(chObj);
				}
				else if (direction == PI_OUT)
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
		xml.readNext();
	}
}


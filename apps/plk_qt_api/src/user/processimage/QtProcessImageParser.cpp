/**
********************************************************************************
\file   QtProcessImageParser.cpp

\brief  Implementation of a process image parser that utilizes a Qt 5.2
		QXmlStreamReader to parse the process image description

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
#include "user/processimage/QtProcessImageParser.h"
#include "user/processimage/IECDataType.h"
#include "common/XmlParserException.h"

/*******************************************************************************
* Private functions
*******************************************************************************/
void QtProcessImageParser::ParseInternal(const char* xmlDescription)
{
	if (!xmlDescription)
	{
		throw std::invalid_argument("Invalid xml file buffer");
	}

	// Adds the xml contents.
	this->xml.addData(xmlDescription);

	while ( !(this->xml.atEnd() || this->xml.hasError()))
	{
		QXmlStreamReader::TokenType token = this->xml.readNext();
		/* If token is StartDocument, Go to next.*/
		if (token == QXmlStreamReader::StartDocument)
		{
			continue;
		}
		if (token == QXmlStreamReader::StartElement)
		{
			if (this->xml.name().compare(QString::fromStdString(
					ProcessImageParser::applicationProcess_element_name)) == 0)
			{
				this->ParseProcessImage();
			}
			else
			{
				XmlParserException ex(GetXmlParserErrorString(XmlParserError::UNEXPECTED_ELEMENT),
									XmlParserError::UNEXPECTED_ELEMENT,
									this->xml.lineNumber(),
									this->xml.columnNumber());
				throw ex;
			}
		}
	}

	if (this->xml.hasError())
	{
		throw XmlParserException(this->xml.errorString().toStdString(),
							XmlParserError::NOT_WELL_FORMED,
							this->xml.lineNumber(),
							this->xml.columnNumber());
	}
}

void QtProcessImageParser::ParseProcessImage()
{
	while (this->xml.readNextStartElement())
	{
		if (this->xml.name().compare(QString::fromStdString(
				ProcessImageParser::processImage_element_name)) == 0)
		{
			this->ParseProcessImageAttributes();
		}
		else
		{
			if (this->xml.hasError())
			{
				throw XmlParserException(this->xml.errorString().toStdString(),
									XmlParserError::UNDEFINED,
									this->xml.lineNumber(),
									this->xml.columnNumber());
			}

			throw XmlParserException(XmlParserError::GetXmlParserErrorString(
										XmlParserError::UNEXPECTED_ELEMENT),
										XmlParserError::UNEXPECTED_ELEMENT,
										this->xml.lineNumber(),
										this->xml.columnNumber());
		}
	}

	if ((this->xml.tokenType() == QXmlStreamReader::EndElement))
	{
		if (this->xml.name().compare(QString::fromStdString(
				ProcessImageParser::applicationProcess_element_name)) == 0)
		{
			// success case.
			return;
		}
		else
		{
			if (this->xml.hasError())
			{
				throw XmlParserException(this->xml.errorString().toStdString(),
									XmlParserError::UNDEFINED,
									this->xml.lineNumber(),
									this->xml.columnNumber());
			}

			throw XmlParserException(XmlParserError::GetXmlParserErrorString(
										XmlParserError::UNEXPECTED_ELEMENT),
										XmlParserError::UNEXPECTED_ELEMENT,
										this->xml.lineNumber(),
										this->xml.columnNumber());
		}
	}
	else
	{
		throw XmlParserException(this->xml.errorString().toStdString(),
							XmlParserError::NOT_WELL_FORMED,
							this->xml.lineNumber(),
							this->xml.columnNumber());
	}
}

void QtProcessImageParser::ParseChannels(Direction::Direction direction)
{
	while (this->xml.readNextStartElement())
	{
		if (this->xml.name().compare(QString::fromStdString(
					ProcessImageParser::channel_element_name)) == 0)
		{
			this->ParseChannelAttributes(direction);
		}
		else
		{
			if (this->xml.hasError())
			{
				throw XmlParserException(this->xml.errorString().toStdString(),
									XmlParserError::UNDEFINED,
									this->xml.lineNumber(),
									this->xml.columnNumber());
			}

			throw XmlParserException(XmlParserError::GetXmlParserErrorString(
										XmlParserError::UNEXPECTED_ELEMENT),
										XmlParserError::UNEXPECTED_ELEMENT,
										this->xml.lineNumber(),
										this->xml.columnNumber());
		}

		if (!(this->xml.readNextStartElement()))
		{
			if ((this->xml.tokenType() == QXmlStreamReader::EndElement))
			{
				if (this->xml.name().compare(QString::fromStdString(
							ProcessImageParser::channel_element_name)) == 0)
				{
					// success case. Where we need to process the remainining channels.
					continue;
				}
				else
				{
					if (this->xml.hasError())
					{
						throw XmlParserException(this->xml.errorString().toStdString(),
											XmlParserError::UNDEFINED,
											this->xml.lineNumber(),
											this->xml.columnNumber());
					}

					throw XmlParserException(XmlParserError::GetXmlParserErrorString(
											XmlParserError::UNEXPECTED_ELEMENT),
											XmlParserError::UNEXPECTED_ELEMENT,
											this->xml.lineNumber(),
											this->xml.columnNumber());
				}
			}
			else
			{
				// Throw error. XML format error.
				throw XmlParserException(this->xml.errorString().toStdString(),
									XmlParserError::NOT_WELL_FORMED,
									this->xml.lineNumber(),
									this->xml.columnNumber());
			}
		}
		else
		{
			if (this->xml.hasError())
			{
				throw XmlParserException(this->xml.errorString().toStdString(),
									XmlParserError::UNDEFINED,
									this->xml.lineNumber(),
									this->xml.columnNumber());
			}

			throw XmlParserException(GetXmlParserErrorString(XmlParserError::UNEXPECTED_ELEMENT),
								XmlParserError::UNEXPECTED_ELEMENT,
								this->xml.lineNumber(),
								this->xml.columnNumber());
		}
	}

	if ((this->xml.tokenType() == QXmlStreamReader::EndElement))
	{
		if (this->xml.name().compare(QString::fromStdString(
					ProcessImageParser::processImage_element_name)) == 0)
		{
			// Success Case. All Channels has been processed.
			return;
		}
		else
		{
			if (this->xml.hasError())
			{
				throw XmlParserException(this->xml.errorString().toStdString(),
									XmlParserError::UNDEFINED,
									this->xml.lineNumber(),
									this->xml.columnNumber());
			}

			throw XmlParserException(GetXmlParserErrorString(XmlParserError::UNEXPECTED_ELEMENT),
								XmlParserError::UNEXPECTED_ELEMENT,
								this->xml.lineNumber(),
								this->xml.columnNumber());
		}
	}
	else
	{
		throw XmlParserException(this->xml.errorString().toStdString(),
							XmlParserError::NOT_WELL_FORMED,
							this->xml.lineNumber(),
							this->xml.columnNumber());
	}
	//TODO while breaks by itself Throw unexpected/xml error.
}

void QtProcessImageParser::ParseProcessImageAttributes()
{
	if (this->xml.hasError())
	{
		throw XmlParserException(this->xml.errorString().toStdString(),
							XmlParserError::UNDEFINED,
							this->xml.lineNumber(),
							this->xml.columnNumber());
	}

	QXmlStreamAttributes attrib = this->xml.attributes();
	Direction::Direction direction = Direction::UNDEFINED;
	UINT byteSize = 0;

	if (attrib.hasAttribute(QString::fromStdString(
			ProcessImageParser::processImage_attribute_Type)))
	{
		QString piType = attrib.value(QString::fromStdString(
							ProcessImageParser::processImage_attribute_Type)).toString();
		direction = (piType == QString::fromStdString(ProcessImageParser::processImage_Type_output)) ? Direction::PI_OUT :
					(piType == QString::fromStdString(ProcessImageParser::processImage_Type_input)) ? Direction::PI_IN : direction;

	}
	else
	{
		XmlParserException ex(ProcessImageParser::processImage_attribute_type_not_found,
							XmlParserError::ATTRIBUTE_NOT_FOUND,
							this->xml.lineNumber(),
							this->xml.columnNumber());
		throw ex;
	}

	if (attrib.hasAttribute(QString::fromStdString(
			ProcessImageParser::processImage_attribute_byteSize)))
	{
		QString size_str = attrib.value(QString::fromStdString(
							ProcessImageParser::processImage_attribute_byteSize)).toString();
		byteSize = size_str.toUInt();
		//TODO check for value != 0.
	}
	else
	{
		XmlParserException ex(ProcessImageParser::processImage_attribute_byteSize_not_found,
							XmlParserError::ATTRIBUTE_NOT_FOUND,
							this->xml.lineNumber(),
							this->xml.columnNumber());
		throw ex;
	}

	if (direction == Direction::PI_OUT)
	{
		this->out.SetSize(byteSize);
		this->ParseChannels(direction);
	}
	else if (direction == Direction::PI_IN)
	{
		this->in.SetSize(byteSize);
		this->ParseChannels(direction);
	}
	else
	{
		XmlParserException ex(ProcessImageParser::processImage_attribute_type_invalid_value,
							XmlParserError::INVALID_ATTRIBUTE_VALUE,
							this->xml.lineNumber(),
							this->xml.columnNumber());
		throw ex;
	}
}

void QtProcessImageParser::ParseChannelAttributes(Direction::Direction direction)
{
	if (this->xml.hasError())
	{
		throw XmlParserException(this->xml.errorString().toStdString(),
							XmlParserError::UNDEFINED,
							this->xml.lineNumber(),
							this->xml.columnNumber());
	}

	std::string name;                       //Name="CN1.X20DO9322.DigitalOutput01"
	IECDataType::IECDataType dataType = IECDataType::UNDEFINED;   //dataType="BITSTRING" from IECDatatype
	UINT byteOffset = 0;                    //PIOffset="0x0000"
	UINT bitOffset = 0;                     //BitOffset="0x00"
	UINT bitSize = 0;                       //dataSize="1"
	/* Get the attributes for Channel */
	QXmlStreamAttributes attributes = this->xml.attributes();

	if (attributes.hasAttribute(QString::fromStdString(
			ProcessImageParser::channel_attribute_name)))
	{
		name = attributes.value(QString::fromStdString(
				 ProcessImageParser::channel_attribute_name)).toString().toStdString();
	}
	else
	{
		XmlParserException ex(ProcessImageParser::channel_attribute_name_not_found,
							XmlParserError::ATTRIBUTE_NOT_FOUND,
							this->xml.lineNumber(),
							this->xml.columnNumber());
		throw ex;
	}

	if (attributes.hasAttribute(QString::fromStdString(
			ProcessImageParser::channel_attribute_dataType)))
	{
		std::string iecDataTypeStr;
		iecDataTypeStr = attributes.value(QString::fromStdString(
							ProcessImageParser::channel_attribute_dataType)).toString().toStdString();
		dataType = IECDataType::GetIECDatatype(iecDataTypeStr);
		// TODO check for invalid value.
	}
	else
	{
		XmlParserException ex(ProcessImageParser::channel_attribute_dataType_not_found,
							XmlParserError::ATTRIBUTE_NOT_FOUND,
							this->xml.lineNumber(),
							this->xml.columnNumber());
		throw ex;
	}

	if (attributes.hasAttribute(QString::fromStdString(
		 ProcessImageParser::channel_attribute_bitSize)))
	{
		bitSize = attributes.value(QString::fromStdString(
					ProcessImageParser::channel_attribute_bitSize)).toString().toUInt();
		// TODO: check for valid bitsize.
	}
	else
	{
		XmlParserException ex(ProcessImageParser::channel_attribute_bitSize_not_found,
							XmlParserError::ATTRIBUTE_NOT_FOUND,
							this->xml.lineNumber(),
							this->xml.columnNumber());
		throw ex;
	}

	if (attributes.hasAttribute(QString::fromStdString(
		 ProcessImageParser::channel_attribute_byteOffset)))
	{
		byteOffset = attributes.value(QString::fromStdString(
						ProcessImageParser::channel_attribute_byteOffset)).toString().toUInt(
						0, 16);
	}
	else
	{
		XmlParserException ex(ProcessImageParser::channel_attribute_byteOffset_not_found,
							XmlParserError::ATTRIBUTE_NOT_FOUND,
							this->xml.lineNumber(),
							this->xml.columnNumber());
		throw ex;
	}

	if (attributes.hasAttribute(QString::fromStdString(
		 ProcessImageParser::channel_attribute_bitOffset)))
	{
		bitOffset = attributes.value(QString::fromStdString(
						ProcessImageParser::channel_attribute_bitOffset)).toString().toUInt(
						0, 16);
	}
	else
	{
		// Not throw ex. Bitoffset may not be available.
	}

	Channel chObj(name, dataType, byteOffset, bitOffset, bitSize,
			direction);

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
		// Invalid direction.
		// Exception has been thrown above.
	}
}


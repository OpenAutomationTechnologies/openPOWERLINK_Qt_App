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
#include <QDebug>

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

	try
	{
		while ( !(this->xml.atEnd() || this->xml.hasError()))
		{
			// QXmlStreamReader::TokenType token = this->xml.readNext();

			QXmlStreamReader::TokenType token = this->xml.readNext();
			/* If token is StartDocument, Go to next.*/
			if (token == QXmlStreamReader::StartDocument)
			{
				continue;
			}
			if (token == QXmlStreamReader::StartElement)
			{
				if (this->xml.name() == QString::fromStdString(
									ProcessImageParser::applicationProcess_element_name).right(-1))
				{
					qDebug("Application process");
					this->ParseProcessImage();
					// qDebug("26: %s", qPrintable(xml.name().toString()));
				}
				else
				{
					qDebug("2");
					qDebug("Unexpected element detected");
					this->RaiseException("Unexpected element detected",
										 XmlParserException::UN_EXPECTED_ELEMENT);
				}
			}
			else
			{
				// qDebug("20: %s", qPrintable(xml.name().toString()));
			}
		}
		if (this->xml.hasError())
		{
			qDebug("3");
			std::string msg = this->xml.errorString().toStdString();
			qDebug("err %s line: %d col:%d ", qPrintable(this->xml.errorString()), this->xml.lineNumber(), this->xml.columnNumber());
			this->RaiseException(msg, XmlParserException::NOT_WELL_FORMED);
		}
	}
	catch(XmlParserException &ex)
	{
		// do cleanups
		this->xml.clear();
		this->in.ResetProcessImage();
		this->out.ResetProcessImage();
		throw;
	}
}

void QtProcessImageParser::ParseProcessImage()
{
	while (this->xml.readNextStartElement())
	{
		if (this->xml.name() == QString::fromStdString(
					 ProcessImageParser::processImage_element_name).right(-1))
		{
			qDebug("Read ProcesImage ");
			this->ParseProcessImageAttributes();
		}
		else
		{
			qDebug("4");
			qDebug("Unexpected element detected");
			this->RaiseException("Unexpected element detected",
								 XmlParserException::UN_EXPECTED_ELEMENT);
		}
	}

	if ((this->xml.tokenType() == QXmlStreamReader::EndElement))
	{
		if (this->xml.name() == QString::fromStdString(
			ProcessImageParser::applicationProcess_element_name).right(-1))
		{
			// success case.
			// qDebug("15");
			return;
		}
		else
		{
			qDebug("5");
			qDebug("Unexpected element detected");
			this->RaiseException("Unexpected element detected",
								 XmlParserException::UN_EXPECTED_ELEMENT);
		}
	}
	else
	{
		qDebug("6");
		this->RaiseException(this->xml.errorString().toStdString(),
							 XmlParserException::NOT_WELL_FORMED);
	}
}

void QtProcessImageParser::ParseChannels(Direction::Direction direction)
{
	while (this->xml.readNextStartElement())
	{
		if (this->xml.name() == QString::fromStdString(
							ProcessImageParser::channel_element_name).right(-1))
		{
			qDebug("Channel, ");
			this->ParseChannelAttributes(direction);
		}
		else
		{
			qDebug("7");
			qDebug("Unexpected element detected");
			this->RaiseException("Unexpected element detected",
								 XmlParserException::UN_EXPECTED_ELEMENT);
		}

		if (!(this->xml.readNextStartElement()))
		{
			// qDebug("NExtStartelement _ ReadChannels : %s", qPrintable(xml.name().toString()));
			if ((this->xml.tokenType() == QXmlStreamReader::EndElement))
			{
				if ((this->xml.name() == QString::fromStdString(
								 ProcessImageParser::channel_element_name).right(-1)) )
				{
					// qDebug("9");
					// success case. Where we need to process the remainining channels.
					continue;
				}
				else
				{
					qDebug("10");
					qDebug("Unexpected element detected");
					this->RaiseException("Unexpected element detected",
										 XmlParserException::UN_EXPECTED_ELEMENT);
				}
			}
			else
			{
				qDebug("11");
				// Throw error. XML format error.
				this->RaiseException(this->xml.errorString().toStdString(),
									 XmlParserException::NOT_WELL_FORMED);
			}
		}
		else
		{
			qDebug("12");
			qDebug("Unexpected start element detected");
			this->RaiseException("Unexpected element detected",
								 XmlParserException::UN_EXPECTED_ELEMENT);
		}
	}

	if ((this->xml.tokenType() == QXmlStreamReader::EndElement))
	{
		if ((this->xml.name() == QString::fromStdString(
					ProcessImageParser::processImage_element_name).right(-1)) )
		{
			// qDebug("8");
			// Success Case. All Channels has been processed.
			return;//check if 'return' is better
		}
		else
		{
			qDebug("13");
			qDebug("Unexpected element detected");
			this->RaiseException("Unexpected element detected",
								 XmlParserException::UN_EXPECTED_ELEMENT);
		}
	}
	else
	{
		qDebug("14");
		// Throw error. XML format error.
		this->RaiseException(this->xml.errorString().toStdString(),
							 XmlParserException::NOT_WELL_FORMED);
	}
	//TODO while breaks by itself Throw unexpected/xml error.
}

void QtProcessImageParser::ParseProcessImageAttributes()
{
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
		qDebug("17");
		std::string message = "ProcessImage attribute '";
		message.append(ProcessImageParser::processImage_attribute_Type);
		message.append("' not found");
		this->RaiseException(message, XmlParserException::ATTRIBUTE_NOT_FOUND);
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
		qDebug("18");
		std::string message = "ProcessImage attribute '";
		message.append(ProcessImageParser::processImage_attribute_byteSize);
		message.append("' not found");
		this->RaiseException(message, XmlParserException::ATTRIBUTE_NOT_FOUND);
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
		qDebug("19");
		std::string message = "ProcessImage attribute '";
		message.append(ProcessImageParser::processImage_attribute_Type);
		message.append("' has invalid value");
		this->RaiseException(message, XmlParserException::INVALID_ATTRIBUTE_VALUE);
	}
}

void QtProcessImageParser::ParseChannelAttributes(Direction::Direction direction)
{
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
		qDebug("21");
		std::string message = "Channel attribute '";
		message.append(ProcessImageParser::channel_attribute_name);
		message.append("' not found.");
		this->RaiseException(message, XmlParserException::ATTRIBUTE_NOT_FOUND);
	}

	if (attributes.hasAttribute(QString::fromStdString(
			ProcessImageParser::channel_attribute_dataType)))
	{
		std::string iecDataTypeStr;
		iecDataTypeStr = attributes.value(QString::fromStdString(
							ProcessImageParser::channel_attribute_dataType)).toString().toStdString();
		dataType = GetIECDatatype(iecDataTypeStr);
		// TODO check for invalid value.
	}
	else
	{
		qDebug("22");
		std::string message = "Channel attribute '";
		message.append(ProcessImageParser::channel_attribute_dataType);
		message.append("' not found.");
		this->RaiseException(message, XmlParserException::ATTRIBUTE_NOT_FOUND);
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
		qDebug("23");
		std::string message = "Channel attribute '";
		message.append(ProcessImageParser::channel_attribute_bitSize);
		message.append("' not found.");
		this->RaiseException(message, XmlParserException::ATTRIBUTE_NOT_FOUND);
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
		qDebug("24");
		std::string message = "Channel attribute '";
		message.append(ProcessImageParser::channel_attribute_byteOffset);
		message.append("' not found.");
		this->RaiseException(message, XmlParserException::ATTRIBUTE_NOT_FOUND);
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

void QtProcessImageParser::RaiseException(std::string message,
								XmlParserException::XmlParserError errCode)
{
	this->xml.raiseError();
	XmlParserException ex(message, errCode, this->xml.lineNumber(), this->xml.columnNumber());
	throw ex;
}

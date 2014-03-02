/**
********************************************************************************
\file   QtProcessImageParser.h

\brief Definitions of a process image parser using the Qt 5.2
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

#ifndef _QTPROCESSIMAGEPARSER_H_
#define _QTPROCESSIMAGEPARSER_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <QXmlStreamReader>

#include "user/processimage/ProcessImageParser.h"
#include "user/processimage/Direction.h"
#include "common/XmlParserException.h"

/**
 * \brief  Inherits ProcessImageParser and implemets using Qt 5.2
 * QXmlStreamReader to incremental parse the ProcessImage xml description.
 *
 * \note Uses XmlParserException to handle the errors.
 *
 */
class QtProcessImageParser : public ProcessImageParser
{

private:

	QXmlStreamReader xml; ///< Xml reader instance

	/**
	 * \brief   Implements the ProcessImage parser.
	 * \param[in] xmlDescription  Char pointer to the xml contents.
	 * \throws std::invalid_argument if xml file buffer is NULL.
	 * \throws XmlParserException If any error occurred.
	 */
	void virtual ParseInternal(const char* xmlDescription);

	/**
	 * \brief   Parses all the ProcessImage tags
	 * \throws XmlParserException If any error occurred.
	 */
	void ParseProcessImage();

	/**
	 * \brief   Parses all the channels and generates the list of Channels.
	 * \param[in] direction  The direction of the ProcessImage channels.
	 * \throws XmlParserException If any error occurred.
	 */
	void ParseChannels(Direction::eDirection direction);

	/**
	 * \brief Parses the attributes present in the ProcessImage tag.
	 * \throws XmlParserException If any error occurred.
	 */
	void ParseProcessImageAttributes();

	/**
	 * \brief Parses the attributes present in the Channel tag.
	 * \param[in] direction  The direction of the ProcessImage channels.
	 * \throws XmlParserException If any error occurred.
	 */
	void ParseChannelAttributes(Direction::eDirection direction);

	/**
	 * \brief Constructs the exception instance and raises exception.
	 * \param[in] message Error message
	 * \param[in] errCode Error code
	 */
	void QtProcessImageParser::RaiseException(const std::string& message,
									XmlParserException::XmlParserErrors errCode);
};

#endif // _QTPROCESSIMAGEPARSER_H_

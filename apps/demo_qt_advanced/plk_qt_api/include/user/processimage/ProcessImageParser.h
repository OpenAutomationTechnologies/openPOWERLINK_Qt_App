/**
********************************************************************************
\file   ProcessImageParser.h

\brief Refer to ProcessImageParser

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

#ifndef _PROCESSIMAGEPARSER_H_
#define _PROCESSIMAGEPARSER_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <string>

#include "user/processimage/Direction.h"
#include "user/processimage/ProcessImage.h"
#include "user/processimage/ProcessImageIn.h"
#include "user/processimage/ProcessImageOut.h"
#include "user/processimage/ProcessImageParserType.h"

#include "common/QtApiGlobal.h"

/**
 * \brief Pure abstract base class provides the definitions used to parse the
 * xml(xap.xml) contents.
 *
 * The user can request the any type of parser available in
 * ProcessImageParserType::ProcessImageParserType to parse the xml.
 */
class PLKQTAPI_EXPORT ProcessImageParser
{

public:
	/**
	 * \brief   Create and return an instance of a ProcessImageParser according to the given type.
	 *
	 * \param[in] type  Type of ProcessImageParser.
	 * \return Returns the requested ProcessImageParser.
	 * \throws std::invalid_argument if the requested ProcessImage parser is not available.
	 */
	static ProcessImageParser* NewInstance(
			const ProcessImageParserType::ProcessImageParserType type);

	/**
	 * \brief   Parses the buffer containing the ProcessImage xml description.
	 *
	 * \param[in] xmlDescription  Pointer to the buffer containing the xml description.
	 *
	 * \note If any exception has occurred the you have to request a new parser
	 * with NewInstance() and "delete" the old one.
	 *
	 * \throws std::invalid_argument if xml file buffer is NULL.
	 * \throws XmlParserException If any error occurred.
	 */
	void Parse(const char* xmlDescription);

	/**
	 * \param[in] direction  The ProcessImage direction
	 * \return Returns the reference to the requested ProcessImage instance.
	 */
	ProcessImage& GetProcessImage(const Direction::Direction direction);

	virtual ~ProcessImageParser();

protected:
	ProcessImageParser();
	ProcessImageIn in;
	ProcessImageOut out;

	static const std::string processImage_attribute_Type;
	static const std::string processImage_attribute_byteSize;

	static const std::string processImage_Type_input;
	static const std::string processImage_Type_output;

	static const std::string channel_attribute_name;
	static const std::string channel_attribute_dataType;
	static const std::string channel_attribute_bitSize;
	static const std::string channel_attribute_byteOffset;
	static const std::string channel_attribute_bitOffset;
	static const std::string applicationProcess_element_name;
	static const std::string processImage_element_name;
	static const std::string channel_element_name;

	static const std::string processImage_attribute_type_not_found;
	static const std::string processImage_attribute_byteSize_not_found;
	static const std::string channel_attribute_name_not_found;
	static const std::string channel_attribute_dataType_not_found;
	static const std::string channel_attribute_bitSize_not_found;
	static const std::string channel_attribute_byteOffset_not_found;
	static const std::string channel_attribute_bitOffset_not_found;

	static const std::string processImage_attribute_type_invalid_value;

private:
	ProcessImageParser(const ProcessImageParser& rhs);

	ProcessImageParser& operator=(const ProcessImageParser& rhs);

	void virtual ParseInternal(const char* xmlDescription) = 0;

};

#endif // _PROCESSIMAGEPARSER_H_

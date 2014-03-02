/**
********************************************************************************
\file   ProcessImageParser.h

\brief Refer to ProcessImageParser

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

#ifndef _PROCESSIMAGEPARSER_H_
#define _PROCESSIMAGEPARSER_H_

#include <fstream>

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
 * ProcessImageParserType::eProcessImageParserType to parse the xml.
 */
class PLKQTAPI_EXPORT ProcessImageParser
{

public:
	/**
	 * \brief   Create and return an instance of a ProcessImageParser according to the given type.
	 *
	 * \param[in] type  Type of ProcessImageParser.
	 * \return Returns the requested ProcessImageParser.
	 * \retval NULL if the requested ProcessImage parser is not available.
	 */
	static ProcessImageParser* NewInstance(
			const ProcessImageParserType::eProcessImageParserType type);

	/**
	 * \brief   Parses the buffer containing the ProcessImage xml description.
	 *
	 * \param[in] xmlDescription  Pointer to the buffer containing the xml description.
	 * \throws std::invalid_argument if xml file buffer is NULL.
	 * \throws XmlParserException If any error occurred.
	 */
	void Parse(const char* description);

	/**
	 * \param[in] direction  The ProcessImage direction
	 * \return Returns the reference to the requested ProcessImage instance.
	 */
	ProcessImage& GetProcessImage(const Direction::eDirection direction);

	virtual ~ProcessImageParser();

protected:
	ProcessImageParser();
	ProcessImageIn in;
	ProcessImageOut out;

	static const std::string piType;
	static const std::string byteSize;

	static const std::string name;
	static const std::string dataType;
	static const std::string bitSize;
	static const std::string byteOffset;
	static const std::string bitOffset;

	static const std::string tagApplicationProcess;
	static const std::string tagProcessImage;
	static const std::string tagChannel;

private:
	ProcessImageParser(const ProcessImageParser& rhs);

	ProcessImageParser& operator=(const ProcessImageParser& rhs);

	void virtual ParseInternal(const char* description) = 0;

};

#endif // _PROCESSIMAGEPARSER_H_

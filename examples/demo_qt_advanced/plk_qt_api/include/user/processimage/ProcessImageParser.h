/**
********************************************************************************
\file   ProcessImageParser.h

\brief  Abstract design of the ProcessImage xml(xap.xml) parser
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
 * \brief
 *
 */
class PLKQTAPI_EXPORT ProcessImageParser
{

public:
	/**
	 * \brief   Create an return an instance of a ProcessImageParser according to the given type.
	 * \param type  Type of ProcessImageParser.
	 * \return ProcessImageParser  The requested ProcessImageParser.
	 */
	static ProcessImageParser* NewInstance(const ProcessImageParserType type);

	/**
	 * \brief   Parse an input filestream containing a ProcessImage description.
	 *
	 * \param description  Input filestream of a file which contains the description of a ProcessImage.
	 */
	void Parse(const char* description);

	/**
	 * \param direction  The ProcessImage direction
	 * \return ProcessImage& The reference to the requested ProcessImage instance.
	 */
	ProcessImage& GetProcessImage(const Direction direction);

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

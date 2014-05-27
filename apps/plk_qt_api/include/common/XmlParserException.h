/**
********************************************************************************
\file   XmlParserException.h

\brief  XML parser exception structure.

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

#ifndef _XML_PARSER_EXCEPTION_H_
#define _XML_PARSER_EXCEPTION_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <stdexcept>
#include <string>

#include <oplk/basictypes.h>

#include <common/QtApiGlobal.h>
#include <common/XmlParserError.h>

/**
 * \brief Inherits std::exception to provide the exception constructs for the
 * XML parsers.
 */
class PLKQTAPI_EXPORT XmlParserException : public std::exception
{
public:

	XmlParserException(const std::string& message,
					   const XmlParserError::XmlParserError errCode,
					   const UINT lineNumber,
					   const UINT colNumber);

	virtual ~XmlParserException() throw();

	/**
	 * \return The formatted string containing the exception message
	 * and the line and column number where error occurred.
	 */
	virtual const char* what() const throw();

	/**
	 * \return The error code.
	 */
	XmlParserError::XmlParserError GetErrorCode() const;

private:
	std::string message;
	const XmlParserError::XmlParserError errCode;
	const UINT lineNumber;
	const UINT colNumber;
};

#endif // _XML_PARSER_EXCEPTION_H_


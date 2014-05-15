/**
********************************************************************************
\file   XmlParserError.cpp

\brief  Provides the methods to access the enum strings.

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
#include "common/XmlParserError.h"

/*******************************************************************************
* Public functions
*******************************************************************************/
namespace XmlParserError
{

std::string GetXmlParserErrorString(XmlParserError errorCode)
{
	std::string errorString;
	switch (errorCode)
	{
		case NOT_WELL_FORMED:
			errorString = "Document not well formed. Check the xml tags.";
			break;
		case PREMATURE_END_OF_DOCUMENT:
			errorString = "Document has nothing to parse";
			break;
		case UNEXPECTED_ELEMENT:
			errorString = "Un-expected element encountered";
			break;
		case UNEXPECTED_ATTRIBUTE:
			errorString = "Un-expected attribute encountered";
			break;
		case ATTRIBUTE_NOT_FOUND:
			errorString = "Attribute not found";
			break;
		case INVALID_ATTRIBUTE_VALUE:
			errorString = "Invalid value for the attribute";
			break;
		case UNDEFINED:
		default:
			errorString = "Undefined error";
	}
	return errorString;
}

} // namespace XmlParserError

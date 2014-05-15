/**
********************************************************************************
\file   IECDataType.h

\brief  The Datatypes derived from the IEC standards.

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

#ifndef _IECDATATYPE_H_
#define _IECDATATYPE_H_
/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <string>

/**
 * \brief IECDataType of the Channels.
 * refer Enum - IECDataType
 */
namespace IECDataType
{
	/**
	 * List of datatypes derived from the IEC standards.
	 */
	enum IECDataType
	{
		UNDEFINED = 0,  ///< Undefined (Used in error handling)
		IEC_BOOL,       ///< Bool,BitString (1 bit)
		IEC_BYTE,       ///< Byte (8 bit)
		IEC_CHAR,       ///< Char (8 bit)
		IEC_WORD,       ///< Word (16 bit)
		IEC_DWORD,      ///< Dword (32 bit)
		IEC_LWORD,      ///< Lword (64 bit)
		IEC_SINT,       ///< Signed short integer (1 byte)
		IEC_INT,        ///< Signed integer (2 byte)
		IEC_DINT,       ///< Double integer (4 byte)
		IEC_LINT,       ///< Long integer (8 byte)
		IEC_USINT,      ///< Unsigned short integer (1 byte)
		IEC_UINT,       ///< Unsigned integer (2 byte)
		IEC_UDINT,      ///< Unsigned double integer (4 byte)
		IEC_ULINT,      ///< Unsigned long integer (8 byte)
		IEC_REAL,       ///< REAL (4 byte)
		IEC_LREAL,      ///< LREAL (8 byte)
		IEC_STRING,     ///< Variable length single byte character string
		IEC_WSTRING     ///< Variable length double byte character string
	};
	/**
	 * \brief   Convert string value to the equivalent IECDataType.
	 * \param[in]  iecDataTypeStr  The string with IEC datatype.
	 * \return The matching IECDataType.
	 */
	IECDataType GetIECDatatype(const std::string& iecDataTypeStr);

} // namespace IECDataType

#endif // _IECDATATYPE_H_

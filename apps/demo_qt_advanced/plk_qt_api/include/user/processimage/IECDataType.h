/**
********************************************************************************
\file   IECDataType.h

\brief  Lists the Datatypes derived from the IEC standards.
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

#ifndef _IECDATATYPE_H_
#define _IECDATATYPE_H_

#include "common/Utility.h"

/**
 * \brief Lists the Datatypes derived from the IEC standards.
 *
 */
enum IECDataType
{
	/**	Removed and managed with BOOL
	 * BITSTRING,	 // BitString (1 bit)
	*/
	IEC_BOOL,		/**< Bool (1 bit) */
	IEC_BYTE,		/**< Byte (8 bit) */
	IEC_CHAR,		/**< Char (8 bit) */
	IEC_DWORD,		/**< Dword (32 bit) */
	IEC_LWORD,		/**< Lword (64 bit) */
	IEC_SINT,		/**< Signed short integer (1 byte) */
	IEC_INT,		/**< Signed integer (2 byte) */
	IEC_DINT,		/**< Double integer (4 byte) */
	IEC_LINT,		/**< Long integer (8 byte) */
	IEC_USINT,		/**< Unsigned short integer (1 byte) */
	IEC_UINT,		/**< Unsigned integer (2 byte) */
	IEC_UDINT,		/**< Unsigned double integer (4 byte) */
	IEC_ULINT,		/**< Unsigned long integer (8 byte) */
	IEC_REAL,		/**< REAL (4 byte) */
	IEC_LREAL,		/**< LREAL (8 byte) */
	IEC_STRING,		/**< Variable length single byte character string */
	IEC_WSTRING		/**< Variable length double byte character string */
};

/**
 * \brief   Convert string value to the equivalent IECDataType.
 * \param[in]  iecDataTypeStr  The string with IEC datatype.
 * \param[out] iecDataType     The DataType from the enum IECDataType.
 * \retval true
 *         false
 */
static bool GetIECDatatype(const std::string& iecDataTypeStr,
								IECDataType& iecDataType)
{

	if (CompareCaseInsensitive("BOOL", iecDataTypeStr)
		|| CompareCaseInsensitive("BITSTRING", iecDataTypeStr) )
	{
		iecDataType = IEC_BOOL;
	}
	else if (CompareCaseInsensitive("BYTE", iecDataTypeStr))
	{
		iecDataType = IEC_BYTE;
	}
	else if (CompareCaseInsensitive("CHAR", iecDataTypeStr))
	{
		iecDataType = IEC_CHAR;
	}
	else if (CompareCaseInsensitive("DWORD", iecDataTypeStr))
	{
		iecDataType = IEC_DWORD;
	}
	else if (CompareCaseInsensitive("LWORD", iecDataTypeStr))
	{
		iecDataType = IEC_LWORD;
	}
	else if (CompareCaseInsensitive("SINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Integer8", iecDataTypeStr))
	{
		iecDataType = IEC_SINT;
	}
	else if (CompareCaseInsensitive("INT", iecDataTypeStr)
			|| CompareCaseInsensitive("Integer16", iecDataTypeStr))
	{
		iecDataType = IEC_INT;
	}
	else if (CompareCaseInsensitive("DINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Integer32", iecDataTypeStr))
	{
		iecDataType = IEC_DINT;
	}
	else if (CompareCaseInsensitive("LINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Integer64", iecDataTypeStr))
	{
		iecDataType = IEC_LINT;
	}
	else if (CompareCaseInsensitive("USINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Unsigned8", iecDataTypeStr))
	{
		iecDataType = IEC_USINT;
	}
	else if (CompareCaseInsensitive("UINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Unsigned16", iecDataTypeStr))
	{
		iecDataType = IEC_UINT;
	}
	else if (CompareCaseInsensitive("UDINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Unsigned32", iecDataTypeStr))
	{
		iecDataType = IEC_UDINT;
	}
	else if (CompareCaseInsensitive("ULINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Unsigned64", iecDataTypeStr))
	{
		iecDataType = IEC_ULINT;
	}
	else if (CompareCaseInsensitive("REAL", iecDataTypeStr))
	{
		iecDataType = IEC_REAL;
	}
	else if (CompareCaseInsensitive("LREAL", iecDataTypeStr))
	{
		iecDataType = IEC_LREAL;
	}
	else if (CompareCaseInsensitive("STRING", iecDataTypeStr))
	{
		iecDataType = IEC_STRING;
	}
	else if (CompareCaseInsensitive("WSTRING", iecDataTypeStr))
	{
		iecDataType = IEC_WSTRING;
	}
	else
	{
		return false;
	}
	return true;
}

#endif // _IECDATATYPE_H_

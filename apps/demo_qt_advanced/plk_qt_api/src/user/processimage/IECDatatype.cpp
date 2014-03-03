/**
********************************************************************************
\file   IECDataType.cpp

\brief  Provides the methods to access the Datatypes derived from the IEC standards.

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
#include "user/processimage/IECDataType.h"
#include "common/Utility.h"

/*******************************************************************************
* Public functions
*******************************************************************************/
namespace IECDataType
{

IECDataType::eIECDataType GetIECDatatype(const std::string& iecDataTypeStr)
{
	IECDataType::eIECDataType iecDataType = IECDataType::UNDEFINED;

	if (CompareCaseInsensitive("BOOL", iecDataTypeStr)
		|| CompareCaseInsensitive("Boolean", iecDataTypeStr)
		|| CompareCaseInsensitive("BITSTRING", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_BOOL;
	}
	else if (CompareCaseInsensitive("BYTE", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_BYTE;
	}
	else if (CompareCaseInsensitive("CHAR", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_CHAR;
	}
	else if (CompareCaseInsensitive("WORD", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_WORD;
	}
	else if (CompareCaseInsensitive("DWORD", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_DWORD;
	}
	else if (CompareCaseInsensitive("LWORD", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_LWORD;
	}
	else if (CompareCaseInsensitive("SINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Integer8", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_SINT;
	}
	else if (CompareCaseInsensitive("INT", iecDataTypeStr)
			|| CompareCaseInsensitive("Integer16", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_INT;
	}
	else if (CompareCaseInsensitive("DINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Integer32", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_DINT;
	}
	else if (CompareCaseInsensitive("LINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Integer64", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_LINT;
	}
	else if (CompareCaseInsensitive("USINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Unsigned8", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_USINT;
	}
	else if (CompareCaseInsensitive("UINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Unsigned16", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_UINT;
	}
	else if (CompareCaseInsensitive("UDINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Unsigned32", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_UDINT;
	}
	else if (CompareCaseInsensitive("ULINT", iecDataTypeStr)
			|| CompareCaseInsensitive("Unsigned64", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_ULINT;
	}
	else if (CompareCaseInsensitive("REAL", iecDataTypeStr)
			|| CompareCaseInsensitive("REAL32", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_REAL;
	}
	else if (CompareCaseInsensitive("LREAL", iecDataTypeStr)
			|| CompareCaseInsensitive("REAL64", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_LREAL;
	}
	else if (CompareCaseInsensitive("STRING", iecDataTypeStr)
			|| CompareCaseInsensitive("VISIBLE_STRING", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_STRING;
	}
	else if (CompareCaseInsensitive("WSTRING", iecDataTypeStr))
	{
		iecDataType = IECDataType::IEC_WSTRING;
	}
	else
	{
		// UnHandled DataType;
	}

	return iecDataType;

} // GetIecDataType

} // namespace IECDatatype

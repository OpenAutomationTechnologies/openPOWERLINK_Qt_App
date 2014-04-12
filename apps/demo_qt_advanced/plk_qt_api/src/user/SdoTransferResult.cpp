/**
********************************************************************************
\file   SdoTransferResult.cpp

\brief  Implementation of the SdoTransferResult class which provides
		methods to access the different attributes of a SdoTransferResult.

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
#include "user/SdoTransferResult.h"
#include <sstream>

const std::map<const UINT32, std::string> SdoTransferResult::CreateAbortCodeStringMap()
{
	std::map<const UINT32, std::string> abortCodeMap;
	abortCodeMap[SDO_AC_TIME_OUT]                          = "SDO protocol timed out";
	abortCodeMap[SDO_AC_UNKNOWN_COMMAND_SPECIFIER]         = "Client/server Command ID not valid or unknown";
	abortCodeMap[SDO_AC_INVALID_BLOCK_SIZE]                = "Invalid block size";
	abortCodeMap[SDO_AC_INVALID_SEQUENCE_NUMBER]           = "Invalid sequence number";
	abortCodeMap[SDO_AC_OUT_OF_MEMORY]                     = "Out of memory";
	abortCodeMap[SDO_AC_UNSUPPORTED_ACCESS]                = "Unsupported access to an object";
	abortCodeMap[SDO_AC_READ_TO_WRITE_ONLY_OBJ]            = "Attempt to read a write-only object";
	abortCodeMap[SDO_AC_WRITE_TO_READ_ONLY_OBJ]            = "Attempt to write a read-only object";
	abortCodeMap[SDO_AC_OBJECT_NOT_EXIST]                  = "Object does not exist in the object dictionary";
	abortCodeMap[SDO_AC_OBJECT_NOT_MAPPABLE]               = "Object cannot be mapped to the PDO";
	abortCodeMap[SDO_AC_PDO_LENGTH_EXCEEDED]               = "The number and length of the objects to be mapped would exceed PDO length";
	abortCodeMap[SDO_AC_GEN_PARAM_INCOMPATIBILITY]         = "General parameter incompatibility";
	abortCodeMap[SDO_AC_INVALID_HEARTBEAT_DEC]             = "Invalid heartbeat declaration";
	abortCodeMap[SDO_AC_GEN_INTERNAL_INCOMPATIBILITY]      = "General internal incompatibility in the device";
	abortCodeMap[SDO_AC_ACCESS_FAILED_DUE_HW_ERROR]        = "Access failed due to an hardware error";
	abortCodeMap[SDO_AC_DATA_TYPE_LENGTH_NOT_MATCH]        = "Data type does not match, length of service parameter does not match";
	abortCodeMap[SDO_AC_DATA_TYPE_LENGTH_TOO_HIGH]         = "Data type does not match, length of service parameter too high";
	abortCodeMap[SDO_AC_DATA_TYPE_LENGTH_TOO_LOW]          = "Data type does not match, length of service parameter too low";
	abortCodeMap[SDO_AC_SUB_INDEX_NOT_EXIST]               = "Sub-index does not exist";
	abortCodeMap[SDO_AC_VALUE_RANGE_EXCEEDED]              = "Value range of parameter exceeded (only for write access)";
	abortCodeMap[SDO_AC_VALUE_RANGE_TOO_HIGH]              = "Value of parameter written too high";
	abortCodeMap[SDO_AC_VALUE_RANGE_TOO_LOW]               = "Value of parameter written too low";
	abortCodeMap[SDO_AC_MAX_VALUE_LESS_MIN_VALUE]          = "Maximum value is less than minimum value";
	abortCodeMap[SDO_AC_GENERAL_ERROR]                     = "General error";
	abortCodeMap[SDO_AC_DATA_NOT_TRANSF_OR_STORED]         = "Data cannot be transferred or stored to the application";
	abortCodeMap[SDO_AC_DATA_NOT_TRANSF_DUE_LOCAL_CONTROL] = "Data cannot be transferred or stored to the application because of local control";
	abortCodeMap[SDO_AC_DATA_NOT_TRANSF_DUE_DEVICE_STATE]  = "Data cannot be transferred or stored to the application because of the present device state";
	abortCodeMap[SDO_AC_OBJECT_DICTIONARY_NOT_EXIST]       = "Object dictionary dynamic generation fails or no object dictionary is present (e.g. object dictionary is generated from file and generation fails because of a file error)";
	abortCodeMap[SDO_AC_CONFIG_DATA_EMPTY]                 = "EDS, DCF or Concise DCF Data set empty";
	return abortCodeMap;
}

const std::map<const UINT32, std::string> SdoTransferResult::abortCodeString = SdoTransferResult::CreateAbortCodeStringMap();


SdoTransferResult::SdoTransferResult()
{

}

SdoTransferResult::SdoTransferResult (const UINT nodeId,
		const UINT index,
		const UINT subIndex,
		const UINT transferredBytes,
		tSdoAccessType sdoAccessType,
		tSdoComConState sdoComConState,
		const UINT32 abortCode) :
		nodeId(nodeId),
		index(index),
		subIndex(subIndex),
		transferredBytes(transferredBytes),
		sdoAccessType(sdoAccessType),
		sdoComConState(sdoComConState),
		abortCode(abortCode)
{

}

UINT SdoTransferResult::GetNodeId() const
{
	return this->nodeId;
}

tSdoComConState SdoTransferResult::GetSdoComConState() const
{
	return this->sdoComConState;
}

UINT32 SdoTransferResult::GetAbortCode() const
{
	return this->abortCode;
}

tSdoAccessType SdoTransferResult::GetSdoAccessType() const
{
	return this->sdoAccessType;
}

UINT SdoTransferResult::GetIndex() const
{
	return this->index;
}

UINT SdoTransferResult::GetSubIndex() const
{
	return this->subIndex;
}

UINT SdoTransferResult::GetTransferredBytes() const
{
	return this->transferredBytes;
}

const std::string SdoTransferResult::GetAbortCodeDescription(const UINT32 abortCode)
{
	std::map<const UINT32, std::string>::const_iterator cIt =
			SdoTransferResult::abortCodeString.find(abortCode);

	if (cIt != SdoTransferResult::abortCodeString.end())
	{
		return cIt->second;
	}
	else
	{
		std::ostringstream oss;
		oss << "Unknown error code detected. Error:0x" << std::hex << abortCode;
		return oss.str();
	}
}

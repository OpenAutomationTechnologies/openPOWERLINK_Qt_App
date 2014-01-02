/**
********************************************************************************
\file   SdoTransferResult.cpp

\brief  Implementation of SdoTransferResult class
*******************************************************************************/

#include "user/SdoTransferResult.h"

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

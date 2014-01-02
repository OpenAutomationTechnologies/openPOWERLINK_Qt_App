/**
********************************************************************************
\file   SdoTransferJob.cpp

\brief  Implementation of SdoTransferJob class
*******************************************************************************/

#include "user/SdoTransferJob.h"

SdoTransferJob::SdoTransferJob(const UINT nodeId,
		const UINT index,
		const UINT subIndex,
		void* data,
		const UINT dataSize,
		tSdoType sdoType,
		tSdoAccessType sdoAccessType) :
		nodeId(nodeId),
		index(index),
		subIndex(subIndex),
		data(data),
		dataSize(dataSize),
		sdoType(sdoType),
		sdoAccessType(sdoAccessType)
{

}

UINT SdoTransferJob::GetNodeId() const
{
	return this->nodeId;
}

UINT SdoTransferJob::GetIndex() const
{
	return this->index;
}

UINT SdoTransferJob::GetSubIndex() const
{
	return this->subIndex;
}

void* SdoTransferJob::GetData() const
{
	return this->data;
}

UINT SdoTransferJob::GetDataSize() const
{
	return this->dataSize;
}

tSdoType SdoTransferJob::GetSdoType() const
{
	return this->sdoType;
}

tSdoAccessType SdoTransferJob::GetSdoAccessType() const
{
	return this->sdoAccessType;
}

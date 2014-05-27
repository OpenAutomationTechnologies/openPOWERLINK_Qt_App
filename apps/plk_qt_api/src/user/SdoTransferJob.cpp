/**
********************************************************************************
\file   SdoTransferJob.cpp

\brief  Implementation of the SdoTransferJob class which provides
		methods to access the different attributes of a SdoTransferJob.

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

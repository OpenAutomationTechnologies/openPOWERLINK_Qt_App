/**
********************************************************************************
\file   SdoTransferJob.h

\brief  Container for holding input parameters used for SDO transfers

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

#ifndef _SDOTRANSFERJOB_H_
#define _SDOTRANSFERJOB_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "oplk/oplk.h"

#include "common/QtApiGlobal.h"

/**
 * \brief This class is used to pass the inputs parameters for the SDO Transfer.
 * \note Refer OplkQtApi::TransferObject
 *
 */
class PLKQTAPI_EXPORT SdoTransferJob
{
public:
	/**
	 * \param[in]      nodeId         Node id to which SDO is to be transferred.
	 * \param[in]      index          The index id to which SDO to be read/write.
	 * \param[in]      subIndex       The subindex id to which SDO to be read/write.
	 * \param[in,out]  data           The pointer to the data
	 * \param[in]      dataSize       The size of the data
	 * \param[in]      sdoType        The protocol type
	 * \param[in]      sdoAccessType  The SDO access type
	 */
	SdoTransferJob(const UINT nodeId,
		const UINT index,
		const UINT subIndex,
		void* const data,
		const UINT dataSize,
		tSdoType sdoType,
		tSdoAccessType sdoAccessType);

	/**
	 * \return Node id of the node.
	 */
	UINT GetNodeId() const;

	/**
	 * \return Index id of the node.
	 */
	UINT GetIndex() const;

	/**
	 * \return SubIndex id of the node.
	 */
	UINT GetSubIndex() const;

	/**
	 * \return Pointer to the data.
	 */
	void* GetData() const;

	/**
	 * \return Size of the data.
	 */
	UINT GetDataSize() const;

	/**
	 * \return Type of the SDO transfer.
	 */
	tSdoType GetSdoType() const;

	/**
	 * \return AccessType of the SDO transfer.
	 */
	tSdoAccessType GetSdoAccessType() const;

private:
	UINT nodeId;
	UINT index;
	UINT subIndex;
	void* data;
	UINT dataSize;
	tSdoType sdoType;
	tSdoAccessType sdoAccessType;
};

#endif // _SDOTRANSFERJOB_H_

/**
********************************************************************************
\file   SdoTransferResult.h

\brief  The result of the SDO Transfer
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

#ifndef _SDOTRANSFERRESULT_H_
#define _SDOTRANSFERRESULT_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include "Epl.h"

#include "common/QtApiGlobal.h"

/**
 * \brief The result of the SDO Transfer
 */
class PLKQTAPI_EXPORT SdoTransferResult
{
public:
	SdoTransferResult();
	/**
	 * \brief   Parameterized constructor with SDO transfer inputs.
	 *
	 * \param[in] nodeId            Node id in which SDO transferred has happened.
	 * \param[in] index             The index id in which SDO transferred has happened.
	 * \param[in] subIndex          The subindex id in which SDO transferred has happened.
	 * \param[in] transferredBytes  The size of the data transferred in bytes.
	 * \param[in] sdoAccessType     The access type of the SDO transfer.
	 * \param[in] sdoComConState    The connection state of the SDO transfer.
	 * \param[in] abortCode         The abortcode if any abort happens.
	 */
	SdoTransferResult(const UINT nodeId,
		const UINT index,
		const UINT subIndex,
		const UINT transferredBytes,
		tSdoAccessType sdoAccessType,
		tSdoComConState sdoComConState,
		const UINT32 abortCode);

	/**
	 * \return UINT  Node id of the node.
	 */
	UINT GetNodeId() const;

	/**
	 * \return tSdoComConState  SDO command layer connection state.
	 */
	tSdoComConState GetSdoComConState() const;

	/**
	 * \return UINT32  Abort code if SDO abort has happened.
	 */
	UINT32 GetAbortCode() const;

	/**
	 * \return tSdoAccessType  AccessType of the SDO transfer.
	 */
	tSdoAccessType GetSdoAccessType() const;

	/**
	 * \return UINT  Index id of the node.
	 */
	UINT GetIndex() const;

	/**
	 * \return UINT  Sub-Index id of the node.
	 */
	UINT GetSubIndex() const;

	/**
	 * \return UINT  The total size of the transferred data in bytes.
	 */
	UINT GetTransferredBytes() const;

private:
	tSdoComConState sdoComConState;
	UINT32 abortCode;
	tSdoAccessType sdoAccessType;
	UINT nodeId;
	UINT index;
	UINT subIndex;
	UINT transferredBytes;
};

#endif // _SDOTRANSFERRESULT_H_

/**
********************************************************************************
\file   SdoTransferResult.h

\brief  The result of the Sdo Transfer
*******************************************************************************/

#ifndef _SDOTRANSFERRESULT_H_
#define _SDOTRANSFERRESULT_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include "Epl.h"

#include "common/QtApiGlobal.h"

/**
 * \brief The result of the Sdo Transfer
 */
class PLKQTAPI_EXPORT SdoTransferResult
{
public:
	SdoTransferResult();
	/**
	 * \brief	Parameterized constructor with SDO transfer inputs.
	 *
	 * \param[in]	nodeId				Node id in which SDO transferred has happened.
	 * \param[in]	index				The index id in which SDO transferred has happened.
	 * \param[in]	subIndex			The subindex id in which SDO transferred has happened.
	 * \param[in]	transferredBytes	The size of the data transferred in bytes.
	 * \param[in]	sdoAccessType		The access type of the SDO transfer.
	 * \param[in]	sdoComConState		The connection state of the SDO transfer.
	 * \param[in]	abortCode			The abortcode if any abort happens.
	 */
	SdoTransferResult(const UINT nodeId,
		const UINT index,
		const UINT subIndex,
		const UINT transferredBytes,
		tSdoAccessType sdoAccessType,
		tSdoComConState sdoComConState,
		const UINT32 abortCode);

	UINT GetNodeId() const;
	tSdoComConState GetSdoComConState() const;
	UINT32 GetAbortCode() const;
	tSdoAccessType GetSdoAccessType() const;
	UINT GetIndex() const;
	UINT GetSubIndex() const;
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

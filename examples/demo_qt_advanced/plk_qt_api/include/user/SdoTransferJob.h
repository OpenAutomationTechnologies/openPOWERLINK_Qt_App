/**
********************************************************************************
\file   SdoTransferJob.h

\brief  The inputs parameters for the Sdo Transfer
*******************************************************************************/

#ifndef _SDOTRANSFERJOB_H_
#define _SDOTRANSFERJOB_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include "Epl.h"

#include "common/QtApiGlobal.h"

/**
 * \brief The inputs parameters for the Sdo Transfer
 */
class PLKQTAPI_EXPORT SdoTransferJob
{
public:
	/**
	 * \brief	Parameterized constructor with SDO transfer inputs.
	 *
	 * \param[in]		nodeId		Node id to which SDO is to be transferred.
	 * \param[in]		index			The index id to which SDO to be read/write.
	 * \param[in]		subIndex		The subindex id to which SDO to be read/write.
	 * \param[in/out]	data			The pointer to the data
	 * \param[in]		dataSize		The size of the data
	 * \param[in]		sdoType			The protocol type
	 * \param[in]		sdoAccessType	The SDO access type
	 */
	SdoTransferJob(const UINT nodeId,
		const UINT index,
		const UINT subIndex,
		void* const data,
		const UINT dataSize,
		tSdoType sdoType,
		tSdoAccessType sdoAccessType);

	UINT GetNodeId() const;
	UINT GetIndex() const;
	UINT GetSubIndex() const;
	void* GetData() const;
	UINT GetDataSize() const;
	tSdoType GetSdoType() const;
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

/**
********************************************************************************
\file   Channel.h

\brief Refer to Channel

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
#ifndef _CHANNEL_H_
#define _CHANNEL_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <string>

#ifdef __unix__
#include <oplk/basictypes.h>
#else
#include "Windows.h"
#endif

#include "common/QtApiGlobal.h"
#include "user/processimage/Direction.h"
#include "user/processimage/IECDataType.h"

/**
 * \brief This class provides an instance of an individual channel in the ProcessImage.
 *
 */
class PLKQTAPI_EXPORT Channel
{
public:
	/**
	 * \brief   Constructs a Channel which inputs the properties of a Channel.
	 *
	 * \param[in] name        Name of the Channel.
	 * \param[in] dataType    Datatype of the Channel.
	 * \param[in] byteOffset  Byte offset of the Channel.
	 * \param[in] bitOffset   Bit offset of the Channel.
	 * \param[in] bitSize     Size of the Channel value in bits.
	 * \param[in] direction   Direction of the Channel.
	 */
	Channel(const std::string& name,
		IECDataType::IECDataType dataType,
		const UINT byteOffset,
		const UINT bitOffset,
		const UINT bitSize,
		Direction::Direction direction);

	/**
	 * \return The name of the Channel.
	 */
	const std::string& GetName() const;

	/**
	 * \return Datatype of the Channel.
	 */
	IECDataType::IECDataType GetDataType() const;

	/**
	 * \return The offset of the Channel within the processimage in bytes.
	 */
	UINT GetByteOffset() const;

	/**
	 * \return The bit offset of the Channel within the processimage.
	 */
	UINT GetBitOffset() const;

	/**
	 * \return The Size of the Channel in bits.
	 */
	UINT GetBitSize() const;

	/**
	 * \return The Direction of the Channel within the processimage.
	 */
	Direction::Direction GetDirection() const;

private:
	std::string name;                       ///< Name
	IECDataType::IECDataType dataType;     ///< dataType
	UINT byteOffset;                        ///< PIOffset
	UINT bitOffset;                         ///< BitOffset
	UINT bitSize;                           ///< dataSize
	Direction::Direction direction;        ///< ProcessImage->type
};

#endif // _CHANNEL_H_

/**
********************************************************************************
\file   ProcessImageIn.h

\brief  Descriptions specific to Input ProcessImage

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

#ifndef _PROCESSIMAGEIN_H_
#define _PROCESSIMAGEIN_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <map>
#include <string>
#include <vector>

#include "user/processimage/Channel.h"
#include "user/processimage/ProcessImage.h"

#include "common/QtApiGlobal.h"

/**
 * \brief Inherits ProcessImage and provides the methods specific to Input ProcessImage
 *
 */
class PLKQTAPI_EXPORT ProcessImageIn : public ProcessImage
{

public:

	ProcessImageIn();

	ProcessImageIn(const UINT byteSize,
		const std::map<std::string, Channel>& channels);

	/**
	 * \brief   Sets the value for a channel in the ProcessImage.
	 *
	 * \param[in] channelName  The name of the Channel.
	 * \param[in] value        The value to be assigned for the Channel.
	 * \throws std::out_of_range  If the requested channel name is not found
	 * \throws std::invalid_argument  If bitoffset exceeds the range.
	 * \throws std::bad_alloc  If the ProcessImage::data pointer is not updated.
	 */
	void SetRawValue(const std::string& channelName,
			std::vector<BYTE>& value);

	/**
	 * \brief   Sets the value for the given byte and bit offsets.
	 *
	 * \param[in] value       The value to be assigned at the given offset.
	 * \param[in] byteOffset  Offset in bytes.
	 * \param[in] bitOffset   Offset in bits with in a single byte
	 *						  (i.e. in the range of 0 to 7).
	 * \throws std::out_of_range  If tried to set value exceeding PI memory range.
	 * \throws std::invalid_argument  If bitoffset exceeds the range.
	 * \throws std::bad_alloc  If the ProcessImage::data pointer is not updated.
	 */
	void SetRawData(const std::vector<BYTE>& value,
			const UINT byteOffset,
			const UINT bitOffset = 0);

private:

	/**
	 * \brief   Overridden function of Add Channel of the ProcessImage Class.
	 *
	 * \param[in] channel  The Channel object
	 * \retval true   If it is added successfully.
	 * \retval false  If it fails to add.
	 */
	bool virtual AddChannelInternal(const Channel& channel);
};

#endif // _PROCESSIMAGEIN_H_

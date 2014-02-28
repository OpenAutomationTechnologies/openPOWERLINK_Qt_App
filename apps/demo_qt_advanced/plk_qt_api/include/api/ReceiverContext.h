/**
********************************************************************************
\file   ReceiverContext.h

\brief  Refer brief of ReceiverContext.

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

#ifndef _RECEIVERCONTEXT_H_
#define _RECEIVERCONTEXT_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <QObject>
#include <QMetaMethod>

/**
 * \brief A container class to pack the receiver and receiver function for
 * handling Async call back events
 */
class ReceiverContext
{
public:
	/**
	 * \brief   Intializer constructor
	 *
	 * Intializes the reciever context object with receiver and receiver function.
	 *
	 * \param[in] receiver          Receiver class object.
	 * \param[in] receiverFunction  Receiver function pointer.
	 */
	ReceiverContext(const QObject* receiver,
		const QMetaMethod* receiverFunction);

	/**
	 * \return Pointer to the receiver object.
	 */
	const QObject* GetReceiver() const;

	/**
	 * \return Pointer to the receiver function.
	 */
	const QMetaMethod* GetReceiverFunction() const;

private:
	const QObject* receiver;
	const QMetaMethod* receiverFunction;
};

#endif // _RECEIVERCONTEXT_H_

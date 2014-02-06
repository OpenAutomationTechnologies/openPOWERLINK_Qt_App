/**
********************************************************************************
\file   OplkQtApi.h

\brief  API to the openPOWERLINK-Stack
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

#ifndef _OPLK_QT_API_H_
#define _OPLK_QT_API_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <QtCore>
#include <string>
#include <QtDebug>
#include <QMetaMethod>
#include <QObject>
#include <QMetaType>

#include "Epl.h"
#include "nmt.h"

#include "api/OplkEventHandler.h"
#include "user/SdoTransferResult.h"
#include "user/SdoTransferJob.h"
#include "api/ReceiverContext.h"
#include "common/QtApiGlobal.h"

#include "user/processimage/ProcessImageIn.h"
#include "user/processimage/ProcessImageOut.h"

/**
 * \brief API to the openPOWERLINK-Stack
 */
class PLKQTAPI_EXPORT OplkQtApi : QObject
{
	Q_OBJECT

public:
	/**
	 * \brief   Initialize openPOWERLINK-Stack with default init-parameters.
	 *          Starts the stack with the given nodeId on the given networkInterface.
	 *
	 * \param[in] nodeId            nodeId to assign to the local stack-instance.
	 * \param[in] networkInterface  network interface to run the openPOWERLINK-Stack on.
	 * \return tEplKernel
	 */
	static tEplKernel InitStack(const UINT nodeId, const std::string& networkInterface);

	/**
	 * \brief   Start openPOWERLINK-Stack.
	 * \return tEplKernel
	 */
	static tEplKernel StartStack();

	/**
	 * \brief   Stop openPOWERLINK-Stack.
	 * \return tEplKernel
	 */
	static tEplKernel StopStack();

	/**
	 * \brief   Sends the NMT command to the specified node.
	 *
	 * \param[in] nodeId      Node id to send the NMT command.
	 * \param[in] nmtCommand  The command to be sent.
	 * \return tEplKernel
	 */
	static tEplKernel ExecuteNmtCommand(const UINT nodeId, tNmtCommand nmtCommand);

	/**
	 * \brief   Handles SDO transfer.
	 *
	 * \param[in] sdoTransferJob    SDO transfer input parameters.
	 * \param[in] receiver          Receiver object.
	 * \param[in] receiverFunction  Receiver function.
	 * \return tEplKernel
	 */
	static tEplKernel TransferObject(
		const SdoTransferJob& sdoTransferJob,
		const QObject& receiver,
		const QMetaMethod& receiverFunction);

	/**
	 * \brief   Allocates the memory for the ProcessImage and sets the
	 *          data pointer to the memory
	 * \param[in,out] in   The instance of the ProcessImageIn
	 * \param[in,out] out  The instance of the ProcessImageOut
	 * \return tEplKernel
	 */
	static tEplKernel SetupProcessImage(ProcessImageIn& in, ProcessImageOut& out);

	/**
	 * \brief   Sets the pointer to the CDC buffer.
	 *
	 *    Note: This function always has precedence over the
	 *          OplkQtApi::SetCdc(const char* cdcFileName), if none of these
	 *          2 functions are called, the API will look for "mnobd.cdc"
	 *          in the working directory.
	 * \param[in] cdcBuffer  Buffer to the CDC contents.
	 * \param[in] size       Size of the buffer.
	 */
	static void SetCdc(const BYTE* cdcBuffer, const UINT size);

	/**
	 * \brief    Sets the CDC file name to the openPOWERLINK stack.
	 * \param cdcFileName  File name of the CDC
	 */
	static void SetCdc(const char* cdcFileName);

	/**
	 * \brief   Sets the Cycle time.
	 *          Note: The user has to do a kNmtCmdSwReset to activate the new cycle time.
	 * \param cycleTime  The requested cycle time.
	 */
	static void SetCycleTime(const long cycleTime);

	/**
	 * \brief   Registers an event-handler for NodeEvents openPOWERLINK-Stack.
	 *
	 *          Note: The receiverFunction should have the same signature as
	 *                OplkEventHandler::SignalNodeFound(const int)
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Object-Function to handle the event.
	 * \retval true   Registration successful.
	 *         false  Registration not successful.
	 */
	static bool RegisterNodeFoundEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief  Un registers an event-handler that is registered for NodeEvents of openPOWERLINK-Stack.
	 * \param[in] receiver          Object that handles the event.
	 * \param[in] receiverFunction  Object-Function that handles the event.
	 * \retval true   Un-registration successful.
	 *         false  Un-registration not successful.
	 */
	static bool UnregisterNodeFoundEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief   Registers an event-handler for state changes of the remote node.
	 *
	 *          Note: The receiverFunction should have the same signature as
	 *                OplkEventHandler::SignalNodeStateChanged(const int, tNmtState)
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Object-Function to handle the event.
	 * \retval true   Registration successful.
	 *         false  Registration not successful.
	 */
	static bool RegisterNodeStateChangedEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief  Un registers an event-handler that is registered for state changes of the remote node.
	 * \param[in] receiver          Object that handles the event.
	 * \param[in] receiverFunction  Object-Function that handles the event.
	 * \retval true   Un-registration successful.
	 *         false  Un-registration not successful.
	 */
	static bool UnregisterNodeStateChangedEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief   Registers an event-handler for state changes of the local node.
	 *
	 *          Note: The receiverFunction should have the same signature as
	 *                OplkEventHandler::SignalLocalNodeStateChanged(tNmtState)
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Object-Function to handle the event.
	 * \retval true   Registration successful.
	 *         false  Registration not successful.
	 */
	static bool RegisterLocalNodeStateChangedEventHandler(
					const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief  Un registers an event-handler that is registered for state changes of the local node.
	 * \param[in] receiver          Object that handles the event.
	 * \param[in] receiverFunction  Object-Function that handles the event.
	 * \retval true   Un-registration successful.
	 *         false  Un-registration not successful.
	 */
	static bool UnregisterLocalNodeStateChangedEventHandler(
					const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief   Registers an event-handler for EventLoggers.
	 *
	 *          Note: The receiverFunction should have the same signature as
	 *                OplkEventHandler::SignalPrintLog(const QString&);
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Object-Function to handle the event.
	 * \retval true   Registration successful.
	 *         false  Registration not successful.
	 */
	static bool RegisterEventLogger(const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief  Un registers an event-handler that is registered for EventLoggers.
	 * \param[in] receiver          Object that handles the event.
	 * \param[in] receiverFunction  Object-Function that handles the event.
	 * \retval true   Un-registration successful.
	 *         false  Un-registration not successful.
	 */
	static bool UnregisterEventLogger(const QObject& receiver,
					const QMetaMethod& receiverFunction);

private:
	static tEplApiInitParam initParam;
	static void SetInitParam();
	OplkQtApi& operator=(const OplkQtApi& api);
	OplkQtApi(const OplkQtApi& api);
	OplkQtApi();
};

#endif //  _OPLK_QT_API_H_

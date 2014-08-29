/**
********************************************************************************
\file   OplkQtApi.h

\brief  Wraps the openPOWERLINK APIs and present it to the user.

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

#ifndef _OPLK_QT_API_H_
#define _OPLK_QT_API_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
//#include <QObject>
#include <QtCore/QMetaMethod>

#include <string>

#include <oplk/oplk.h>
#include <oplk/nmt.h>
#include <user/nmtu.h>

#include "user/SdoTransferResult.h"
#include "user/SdoTransferJob.h"
#include "api/ReceiverContext.h"
#include "common/QtApiGlobal.h"
#include "user/processimage/ProcessImageIn.h"
#include "user/processimage/ProcessImageOut.h"

/**
 * \brief Class provides the interface to the user to use the API's of the openPOWERLINK stack.
 *
 * OplkQtApi class provides the easy to use interface to use the API's of the
 * openPOWERLINK stack v2. It also provides the possibility to register and
 * unregister for various events triggered by the stack.
 */
class PLKQTAPI_EXPORT OplkQtApi// : QObject // TODO: check if we need
{
//	Q_OBJECT

public:
	/**
	 * \brief   Initialize openPOWERLINK-Stack with default init-parameters.
	 *
	 * Sets the default configuration parameters to stack.
	 *
	 * \param[in] nodeId            Id assigned to the node (ie. The local stack-instance; referred as local node-id)
	 * \param[in] networkInterface  Network interface.
	 */
	static tOplkError InitStack(const UINT nodeId,
								const std::string& networkInterface);

	/**
	 * \brief   Start openPOWERLINK-Stack.
	 */
	static tOplkError StartStack();

	/**
	 * \brief   Stop openPOWERLINK-Stack.
	 */
	static tOplkError StopStack();

	/**
	 * \brief   Send a NMT command to the specified node.
	 *
	 * \param[in] nodeId      Node id to send the NMT command.
	 * \param[in] nmtCommand  The command to be sent.
	 */
	static tOplkError ExecuteNmtCommand(UINT nodeId,
										tNmtCommand nmtCommand);

	/**
	 * \brief   Allows the user to perform an SDO transfer via the oplk network
	 *
	 * Performs SDO read and write on both local and remote nodes.
	 * - For the remote nodes the user will receive the result of the transfer
	 * in the receiverFunction.
	 *
	 * \param[in] sdoTransferJob    SDO transfer input parameters.
	 * \param[in] receiver          Receiver object.
	 * \param[in] receiverFunction  Receiver function where the result is received.
	 *
	 * \see OplkEventHandler::SignalSdoTransferFinished(const SdoTransferResult);
	 */
	static tOplkError TransferObject(const SdoTransferJob& sdoTransferJob,
									const QObject& receiver,
									const QMetaMethod& receiverFunction);

	/**
	 * \brief   Allocates the memory for the ProcessImage and updates the
	 *          data pointer
	 * \param[in,out] in   The instance of the ProcessImageIn
	 * \param[in,out] out  The instance of the ProcessImageOut
	 */
	static tOplkError AllocateProcessImage(ProcessImageIn& in,
										   ProcessImageOut& out);

	/**
	 * \brief   Sets the pointer to the CDC buffer.
	 *
	 * \param[in] cdcBuffer  Buffer to the CDC contents.
	 * \param[in] size       Size of the buffer in bytes.
	 *
	 * \note This function always has precedence over the
	 *       OplkQtApi::SetCdc(const std:string&), if none of these two
	 *       functions are called, the stack is set to look for "mnobd.cdc" in the
	 *       working directory.
	 */
	static tOplkError SetCdc(const BYTE* cdcBuffer, const UINT size);

	/**
	 * \brief    Set path to CDC file.
	 * \param[in] cdcFileName  File name of the CDC
	 * \note The function OplkQtApi::SetCdc(const BYTE*, const UINT) always has
	 *       precedence over this function. If none of these two functions are
	 *       called, the stack is set to look for "mnobd.cdc" in the working directory.
	 */
	static tOplkError SetCdc(const std::string& cdcFileName);

	/**
	 * \param[in] cycleTime  POWERLINK cycle time in microseconds.
	 * \note The cycleTime will be active after an NmtSwReset to the local node.
	 */
	static tOplkError SetCycleTime(const ULONG cycleTime);

	/**
	 * \brief Registers for the node found events of the stack.
	 *
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Registration successful.
	 * \retval false  Registration not successful.
	 *
	 * \see OplkEventHandler::SignalNodeFound(const int)
	 */
	static bool RegisterNodeFoundEventHandler(const QObject& receiver,
									const QMetaMethod& receiverFunction);

	/**
	 * \brief Unregisters the receiver from receiving the node found events of stack.
	 *
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Unregistration successful.
	 * \retval false  Unregistration not successful.
	 *
	 * \see OplkEventHandler::SignalNodeFound(const int)
	 */
	static bool UnregisterNodeFoundEventHandler(const QObject& receiver,
									const QMetaMethod& receiverFunction);

	/**
	 * \brief Registers for the state change events of the remote node.
	 *
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Registration successful.
	 * \retval false  Registration not successful.
	 *
	 * \see OplkEventHandler::SignalNodeStateChanged(const int, tNmtState)
	 */
	static bool RegisterNodeStateChangedEventHandler(const QObject& receiver,
									const QMetaMethod& receiverFunction);

	/**
	 * \brief Unregisters the receiver from receiving the state change events of the remote node.
	 *
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Unregistration successful.
	 * \retval false  Unregistration not successful.
	 *
	 * \see OplkEventHandler::SignalNodeStateChanged(const int, tNmtState)
	 */
	static bool UnregisterNodeStateChangedEventHandler(const QObject& receiver,
									const QMetaMethod& receiverFunction);

	/**
	 * \brief Registers for the state change events of the local node.
	 *
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Registration successful.
	 * \retval false  Registration not successful.
	 *
	 * \note The state indicate the NMT state changes of the local node.
	 *
	 * \see OplkEventHandler::SignalLocalNodeStateChanged(tNmtState)
	 */
	static bool RegisterLocalNodeStateChangedEventHandler(const QObject& receiver,
									const QMetaMethod& receiverFunction);

	/**
	 * \brief Unregisters the receiver from receiving the state change events of the local node.
	 *
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Unregistration successful.
	 * \retval false  Unregistration not successful.
	 *
	 * \see OplkEventHandler::SignalLocalNodeStateChanged(tNmtState)
	 */
	static bool UnregisterLocalNodeStateChangedEventHandler(const QObject& receiver,
									const QMetaMethod& receiverFunction);

	/**
	 * \brief Registers for the formatted log events from the stack.
	 *
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Registration successful.
	 * \retval false  Registration not successful.
	 *
	 * \see OplkEventHandler::SignalPrintLog(const QString&)
	 */
	static bool RegisterEventLogger(const QObject& receiver,
									const QMetaMethod& receiverFunction);

	/**
	 * \brief Unregisters from receiving the formatted log events from the stack.
	 *
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Unregistration successful.
	 * \retval false  Unregistration not successful.
	 *
	 * \see OplkEventHandler::SignalPrintLog(const QString&);
	 */
	static bool UnregisterEventLogger(const QObject& receiver,
									const QMetaMethod& receiverFunction);

	/**
	 * \brief Registers for the sync events from the stack.
	 *
	 * \param[in] direction         The direction of the processimage.
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Registration successful.
	 * \retval false  Registration not successful.
	 *
	 * \see OplkSyncEventHandler::SignalUpdateInputValues()
	 * \see OplkSyncEventHandler::SignalUpdatedOutputValues()
	 */
	static bool RegisterSyncEventHandler(Direction::Direction direction,
										 const QObject& receiver,
										 const QMetaMethod& receiverFunction);

	/**
	 * \brief Unregisters from receiving the sync events from the stack.
	 *
	 * \param[in] direction         The direction of the processimage.
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Unregistration successful.
	 * \retval false  Unregistration not successful.
	 *
	 * \see OplkSyncEventHandler::SignalUpdateInputValues()
	 * \see OplkSyncEventHandler::SignalUpdatedOutputValues()
	 */
	static bool UnregisterSyncEventHandler(Direction::Direction direction,
										 const QObject& receiver,
										 const QMetaMethod& receiverFunction);

	/**
	 * \return The ProcessImage sync wait time in micro seconds.
	 */
	static ULONG GetSyncWaitTime();

	/**
	 * \brief Sets the ProcessImage sync to wait for the given time in microseconds.
	 *
	 * \param[in] sleepTime Processimage sync thread sleep time in micro seconds.
	 *
	 * \note It defaults to OplkSyncEventHandler::sleepTime value
	 */
	static void SetSyncWaitTime(const ULONG sleepTime);

	/**
	 * \brief Registers the receiver for receiving the sync wait time change events.
	 *
	 * Registers for the change events of the sync wait time.
	 *
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Registration successful.
	 * \retval false  Registration not successful.
	 *
	 * \see OplkQtApi::SetSyncWaitTime(const ULONG)
	 * \see OplkSyncEventHandler::SignalSyncWaitTimeChanged(ulong)
	 */
	static bool RegisterSyncWaitTimeChangedEventHandler(const QObject& receiver,
										const QMetaMethod& receiverFunction);

	/**
	 * \brief Unregisters the receiver from receiving the sync wait time change events.
	 *
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Unregistration successful.
	 * \retval false  Unregistration not successful.
	 *
	 * \see OplkQtApi::SetSyncWaitTime(const ULONG)
	 * \see OplkSyncEventHandler::SignalSyncWaitTimeChanged(ulong)
	 */
	static bool UnregisterSyncWaitTimeChangedEventHandler(const QObject& receiver,
										const QMetaMethod& receiverFunction);

	/**
	 * \brief Registers for the critical error events from the stack.
	 *
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Registration successful.
	 * \retval false  Registration not successful.
	 *
	 * \see OplkEventHandler::SignalCriticalError(const QString&)
	 */
	static bool RegisterCriticalErrorEventHandler(const QObject& receiver,
										const QMetaMethod& receiverFunction);

	/**
	 * \brief Unregisters from receiving the critical error events from the stack.
	 *
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Member function to handle the event.
	 * \retval true   Unregistration successful.
	 * \retval false  Unregistration not successful.
	 *
	 * \see OplkEventHandler::SignalCriticalError(const QString&)
	 */
	static bool UnregisterCriticalErrorEventHandler(const QObject& receiver,
										const QMetaMethod& receiverFunction);

private:
	static tOplkApiInitParam initParam;
	static bool cdcSet;

	OplkQtApi();
	OplkQtApi(const OplkQtApi& api);
	OplkQtApi& operator=(const OplkQtApi& api);

	static void SetInitParam();
};

#endif //  _OPLK_QT_API_H_

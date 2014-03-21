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
#include <QMetaMethod>

#include <string>

#include <oplk/oplk.h>
#include <oplk/nmt.h>

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
	 * \param[in] nodeId            nodeId to be assigned to the local stack-instance. (Referred as Local NodeId)
	 * \param[in] networkInterface  network interface .
	 * \return Returns a tOplkError error code.
	 */
	static tOplkError InitStack(const UINT nodeId, const std::string& networkInterface);

	/**
	 * \brief   Start openPOWERLINK-Stack.
	 * \return Returns a tOplkError error code.
	 */
	static tOplkError StartStack();

	/**
	 * \brief   Stop openPOWERLINK-Stack.
	 * \return Returns a tOplkError error code.
	 */
	static tOplkError StopStack();

	/**
	 * \brief   Send a NMT command to the specified node.
	 *
	 * \param[in] nodeId      Node id to send the NMT command.
	 * \param[in] nmtCommand  The command to be sent.
	 * \return Returns a tOplkError error code.
	 */
	static tOplkError ExecuteNmtCommand(const UINT nodeId, tNmtCommand nmtCommand);

	/**
	 * \brief   Allows the user to perform an SDO transfer via the oplk network
	 *
	 * Performs SDO read and write on both local and remote nodes.
	 * - For remote nodes the user will receive the result of the transfer in
	 * the receiverFunction after the transfer has been completed.
	 * - The receiver function should have the same signature as
	 * OplkEventHandler::SignalSdoTransferFinished(const SdoTransferResult);
	 *
	 * \param[in] sdoTransferJob    SDO transfer input parameters.
	 * \param[in] receiver          Receiver object.
	 * \param[in] receiverFunction  Receiver function.
	 * \return Returns a tOplkError error code.
	 */
	static tOplkError TransferObject(
		const SdoTransferJob& sdoTransferJob,
		const QObject& receiver,
		const QMetaMethod& receiverFunction);

	/**
	 * \brief   Allocate the memory for the ProcessImage and updates the
	 *          data pointer
	 * \param[in,out] in   The instance of the ProcessImageIn
	 * \param[in,out] out  The instance of the ProcessImageOut
	 * \return Returns a tOplkError error code.
	 */
	static tOplkError AllocateProcessImage(ProcessImageIn& in, ProcessImageOut& out);

	/**
	 * \brief   Sets the pointer to the CDC buffer.
	 *
	 * \note This function always has precedence over the
	 *       OplkQtApi::SetCdc(const char* cdcFileName), if none of these two
	 *       functions are called, the API will look for "mnobd.cdc" in the
	 *       working directory.
	 * \param[in] cdcBuffer  Buffer to the CDC contents.
	 * \param[in] size       Size of the buffer in bytes.
	 * \return Returns a tOplkError error code.
	 */
	static tOplkError SetCdc(const BYTE* cdcBuffer, const UINT size);

	/**
	 * \brief    Set path to CDC file.
	 *
	 * \note The function OplkQtApi::SetCdc(const BYTE*, const UINT) always has
	 *       precedence over this function. If none of these two functions are
	 *       called, the API will look for "mnobd.cdc" in the working directory.
	 *
	 * \param cdcFileName  File name of the CDC
	 * \return Returns a tOplkError error code.
	 */
	static tOplkError SetCdc(const char* cdcFileName);

	/**
	 * \brief   Sets the Cycle time in micro seconds.
	 * \note The user has execute an NMT command 'kNmtCmdSwReset' to activate the new cycle time.
	 * \param cycleTime  The requested cycle time.
	 * \return Returns a tOplkError error code.
	 */
	static tOplkError SetCycleTime(const ULONG cycleTime);

	/**
	 * \brief    Subscribes for the NodeFound signal.
	 *
	 * The receiverFuncion will be registered for the NodeFound signal.
	 *
	 * \note The receiverFunction should have the same signature as
	 *        OplkEventHandler::SignalNodeFound(const int)
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Object-Function to handle the event.
	 * \retval true   Registration successful.
	 * \retval false  Registration not successful.
	 */
	static bool RegisterNodeFoundEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief  Un subscribers the receiver from receiving the NodeFound signal.
	 *
	 * \note The receiverFunction should have the same signature as
	 *        OplkEventHandler::SignalNodeFound(const int)
	 * \param[in] receiver          Object that handles the event.
	 * \param[in] receiverFunction  Object-Function that handles the event.
	 * \retval true   Un-registration successful.
	 * \retval false  Un-registration not successful.
	 */
	static bool UnregisterNodeFoundEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief   Subscribes for the NodeStateChanged signal.
	 *
	 * \note NodeStateChanged signals indicate the NMT state changes of the remote nodes.
	 * \note The receiverFunction should have the same signature as
	 *       OplkEventHandler::SignalNodeStateChanged(const int, tNmtState)
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Object-Function to handle the event.
	 * \retval true   Registration successful.
	 * \retval false  Registration not successful.
	 */
	static bool RegisterNodeStateChangedEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief  Un subscribers the receiver from receiving the NodeStateChanged signals.
	 * \note The receiverFunction should have the same signature as
	 *       OplkEventHandler::SignalNodeStateChanged(const int, tNmtState)
	 * \param[in] receiver          Object that handles the event.
	 * \param[in] receiverFunction  Object-Function that handles the event.
	 * \retval true   Un-registration successful.
	 * \retval false  Un-registration not successful.
	 */
	static bool UnregisterNodeStateChangedEventHandler(const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief   Subscribes for the LocalNodeStateChanged signal.
	 *
	 * \note LocalNodeStateChanged signals indicate the NMT state changes of the local node.
	 * \note The receiverFunction should have the same signature as
	 *       OplkEventHandler::SignalLocalNodeStateChanged(tNmtState)
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Object-Function to handle the event.
	 * \retval true   Registration successful.
	 * \retval false  Registration not successful.
	 */
	static bool RegisterLocalNodeStateChangedEventHandler(
					const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief  Un subscribers the receiver from receiving the LocalNodeStateChanged signals.
	 * \note The receiverFunction should have the same signature as
	 *       OplkEventHandler::SignalLocalNodeStateChanged(tNmtState)
	 * \param[in] receiver          Object that handles the event.
	 * \param[in] receiverFunction  Object-Function that handles the event.
	 * \retval true   Un-registration successful.
	 * \retval false  Un-registration not successful.
	 */
	static bool UnregisterLocalNodeStateChangedEventHandler(
					const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief   Subscribes for the PrintLog signals.
	 *
	 * \note The receiverFunction should have the same signature as
	 *       OplkEventHandler::SignalPrintLog(const QString&);
	 * \param[in] receiver          Object to handle the event.
	 * \param[in] receiverFunction  Object-Function to handle the event.
	 * \retval true   Registration successful.
	 * \retval false  Registration not successful.
	 */
	static bool RegisterEventLogger(const QObject& receiver,
					const QMetaMethod& receiverFunction);

	/**
	 * \brief   Un subscribes the receiver from receiving the Printlog signals.
	 * \note The receiverFunction should have the same signature as
	 *       OplkEventHandler::SignalPrintLog(const QString&);
	 * \param[in] receiver          Object that handles the event.
	 * \param[in] receiverFunction  Object-Function that handles the event.
	 * \retval true   Un-registration successful.
	 * \retval false  Un-registration not successful.
	 */
	static bool UnregisterEventLogger(const QObject& receiver,
					const QMetaMethod& receiverFunction);

private:
	static tEplApiInitParam initParam;
	static bool cdcSet;

	OplkQtApi();
	OplkQtApi(const OplkQtApi& api);
	OplkQtApi& operator=(const OplkQtApi& api);

	static void SetInitParam();
};

#endif //  _OPLK_QT_API_H_

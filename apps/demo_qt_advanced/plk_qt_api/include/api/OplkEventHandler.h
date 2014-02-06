/**
********************************************************************************
\file   OplkEventHandler.h

\brief  Event handler for the POWERLINK events
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

#ifndef _OPLK_EVENTHANDLER_H_
#define _OPLK_EVENTHANDLER_H_

#include <Epl.h>
#include <nmt.h>
#include <EplCfg.h>

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QString>
#include <QDateTime>

#include "api/ReceiverContext.h"
#include "user/SdoTransferResult.h"

/**
 * \brief Thread used to receive openPOWERLINK-Stack events.
 *
 *        This class is intended to _only_ be used by OplkQtApi, clients can use OplkQtApi::RegisterEventHandler().
 */
class OplkEventHandler : public QThread
{
	Q_OBJECT

	friend class OplkQtApi;

public:

	/**
	 * \brief   Waits until the NMT state NMT_GS_OFF is reached
	 *
	 */
	void AwaitNmtGsOff();

	/**
	 * \brief   Returns the address of the event callback function.
	 *
	 * \return tEplApiCbEvent  Returns the address of event callback function
	 */
	tEplApiCbEvent GetEventCbFunc(void);

private:
	QMutex          mutex;
	QWaitCondition  nmtGsOffCondition;

	/**
	 * \brief   Event callback function
	 *
	 * \param[in] eventType  Type of event
	 * \param[in] eventArg   Pointer to union which describes the event in detail
	 * \param[in] userArg    User specific argument
	 * \return tEplKernel    Returns a tEplKernel error code.
	 */
	static tEplKernel AppCbEvent(tEplApiEventType eventType, tEplApiEventArg* eventArg, void GENERIC* userArg);

	/**
	 * \brief   Process the openPOWERLINK Stack NMT state change events
	 *
	 * \param[in] nmtStateChange  Details of the NMT state changes
	 * \param[in] userArg         User specific argument
	 * \return tEplKernel         Returns a tEplKernel error code.
	 */
	tEplKernel ProcessNmtStateChangeEvent(tEventNmtStateChange* nmtStateChange, void GENERIC* userArg);

	/**
	 * \brief   Process error and warning events
	 *
	 * \param[in] internalError  Details of the error and warning events.
	 * \param[in] userArg        User specific argument.
	 * \return tEplKernel        Returns a tEplKernel error code.
	 */
	tEplKernel ProcessErrorWarningEvent(tEplEventError* internalError, void GENERIC* userArg);

	/**
	 * \brief   Process history events
	 *
	 * \param[in] historyEntry  Details of the history events
	 * \param[in] userArg       User specific argument
	 * \return tEplKernel       Returns a tEplKernel error code.
	 */
	tEplKernel ProcessHistoryEvent(tEplErrHistoryEntry* historyEntry, void GENERIC* userArg);

	/**
	 * \brief   Process node events
	 *
	 * \param[in] nodeEvent  Details of the node events.
	 * \param[in] userArg    User specific argument.
	 * \return tEplKernel    Returns a tEplKernel error code.
	 */
	tEplKernel ProcessNodeEvent(tEplApiEventNode* nodeEvent, void GENERIC* userArg);

	/**
	 * \brief   Process SDO events
	 *
	 * \param[in] sdoEvent  Details of the SDO events.
	 * \param[in] userArg   User specific argument
	 * \return tEplKernel   Returns a tEplKernel error code.
	 */
	tEplKernel ProcessSdoEvent(tSdoComFinished* sdoEvent, void GENERIC* userArg);

	/**
	 * \brief   Process CFM progress events
	 *
	 * \param[in] cfmProgress  Details of the CFM progress events.
	 * \param[in] userArg      User specific argument.
	 * \return tEplKernel      Returns a tEplKernel error code.
	 */
	tEplKernel ProcessCfmProgressEvent(tCfmEventCnProgress* cfmProgress, void GENERIC* userArg);

	/**
	 * \brief   Process CFM result events
	 *
	 * \param[in] cfmResult  Result of the CFM event occurred.
	 * \param[in] userArg    User specific argument.
	 * \return tEplKernel    Returns a tEplKernel error code.
	 */
	tEplKernel ProcessCfmResultEvent(tEplApiEventCfmResult* cfmResult, void GENERIC* userArg);

	/**
	 * \brief   Triggers a signal `SignalNodeFound` when an ident response is received from a node.
	 *
	 * \param[in] nodeId  The id of the respective node
	 */
	void TriggerNodeFound(const int nodeId);

	/**
	 * \brief   Triggers a signal `SignalNodeStateChanged` when there is a change in the state of the node.
	 *
	 * \param[in] nodeId    The id of the respective node
	 * \param[in] nmtState  The changed state of the respective node
	 */
	void TriggerNodeStateChanged(const int nodeId, tNmtState nmtState);

	/**
	 * \brief   Triggers a signal `SignalLocalNodeStateChanged` when there is a change of the state of the stack.
	 *
	 * \param[in] nmtState  The new state to which the local node has changed to.
	 */
	void TriggerLocalNodeStateChanged(tNmtState nmtState);

	/**
	 * \brief   Triggers a signal `SignalPrintLog` with the log message
	 *
	 * \param[in] logStr  Log message.
	 */
	void TriggerPrintLog(const QString logStr);

	/**
	 * \brief   Triggers a signal `SignalSdoTransferFinished` when SDO transfer finished.
	 *
	 * \param[in] result               The result of the SDO transfer.
	 * \param[in,out] receiverContext  The details of the reciver object.
	 */
	void TriggerSdoTransferFinished(const tSdoComFinished& result, const ReceiverContext* receiverContext);

	/**
	 * \brief   Set Default node assignment when CFM manager is not available.
	 */
	tEplKernel SetDefaultNodeAssignment();

	/**
	 * \brief   Returns the instance of the class
	 *
	 * \return OplkEventHandler
	 */
	static OplkEventHandler& GetInstance();

	OplkEventHandler();
	OplkEventHandler(const OplkEventHandler& eventHandler);
	OplkEventHandler& operator=(const OplkEventHandler& eventHandler);

signals:
	/**
	 * \brief   Node found signal
	 *
	 * \param[in] nodeId  Node id of the node.
	 */
	void SignalNodeFound(const int nodeId);

	/**
	 * \brief   The NMT state of any remote node changes.
	 *
	 * \param[in] nodeId    Node id of the node.
	 * \param[in] nmtState  The new NMT state to which it has changed.
	 */
	void SignalNodeStateChanged(const int nodeId, tNmtState nmtState);

	/**
	 * \brief   The NMT state of the local node.
	 *
	 * \param[in] nmtState  The new NMT state to which it has changed.
	 */
	void SignalLocalNodeStateChanged(tNmtState nmtState);

	/**
	 * \brief   Log message of the events occurring
	 *
	 * \param[in] logStr  Detailed message
	 */
	void SignalPrintLog(const QString &logStr);

	/**
	 * \brief   Signals after the SDO transfer has happened/aborted.
	 *
	 * \param[in] result  Result of the SDO transfer query.
	 */
	void SignalSdoTransferFinished(const SdoTransferResult result);

};

#endif // _OPLK_EVENTHANDLER_H_

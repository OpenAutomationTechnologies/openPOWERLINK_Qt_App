/**
********************************************************************************
\file   OplkEventHandler.h

\brief  Event handler for the POWERLINK events
*******************************************************************************/

#ifndef _OPLK_EVENTHANDLER_H_
#define _OPLK_EVENTHANDLER_H_

#include <Epl.h>
#include <nmt.h>

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QString>
#include <QDateTime>

#include "common/global.h"

/**
 * \brief Thread used to receive openPOWERLINK-Stack events.
 *
 *        This class is intended to _only_ be used by OplkQtApi, clients can use OplkQtApi::RegisterEventHandler().
 */
class PLKQTAPI_EXPORT OplkEventHandler : public QThread
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
	 * \return tEplApiCbEvent   Returns the address of event callback function
	 */
	tEplApiCbEvent GetEventCbFunc(void);

private:
	QMutex          mutex;
	QWaitCondition  nmtGsOffCondition;

	/**
	 * \brief   Event callback function
	 *
	 * \param eventType     Type of event
	 * \param eventArg      Pointer to union which describes the event in detail
	 * \param userArg       User specific argument
	 * \return tEplKernel   The function returns a tEplKernel error code.
	 */
	static tEplKernel AppCbEvent(tEplApiEventType eventType, tEplApiEventArg* eventArg, void GENERIC* userArg);

	/**
	 * \brief   Process the openPOWERLINK Stack NMT state change events
	 *
	 * \param nmtStateChange    Details of the NMT state changes
	 * \param userArg           User specific argument
	 * \return tEplKernel       Returns a tEplKernel error code.
	 */
	tEplKernel ProcessNmtStateChangeEvent(tEventNmtStateChange* nmtStateChange, void GENERIC* userArg);

	/**
	 * \brief   Process error and warning events
	 *
	 * \param internalError		Details of the error and warning events.
	 * \param userArg           User specific argument.
	 * \return tEplKernel       Returns a tEplKernel error code.
	 */
	tEplKernel ProcessErrorWarningEvent(tEplEventError* internalError, void GENERIC* userArg);

	/**
	 * \brief   Process history events
	 *
	 * \param historyEntry		Details of the history events
	 * \param userArg           User specific argument
	 * \return tEplKernel       Returns a tEplKernel error code.
	 */
	tEplKernel ProcessHistoryEvent(tEplErrHistoryEntry* historyEntry, void GENERIC* userArg);

	/**
	 * \brief   Process node events
	 *
	 * \param nodeEvent			Details of the node events.
	 * \param userArg           User specific argument.
	 * \return tEplKernel       Returns a tEplKernel error code.
	 */
	tEplKernel ProcessNodeEvent(tEplApiEventNode* nodeEvent, void GENERIC* userArg);

	/**
	 * \brief   Process SDO events
	 *
	 * \param sdoEvent			Details of the SDO events.
	 * \param userArg           User specific argument
	 * \return tEplKernel       Returns a tEplKernel error code.
	 */
	tEplKernel ProcessSdoEvent(tSdoComFinished* sdoEvent, void GENERIC* userArg);

	/**
	 * \brief   Process CFM progress events
	 *
	 * \param cfmProgress		Details of the CFM progress events.
	 * \param userArg           User specific argument.
	 * \return tEplKernel       Returns a tEplKernel error code.
	 */
	tEplKernel ProcessCfmProgressEvent(tCfmEventCnProgress* cfmProgress, void GENERIC* userArg);

	/**
	 * \brief   Process CFM result events
	 *
	 * \param cfmResult			Result of the CFM event occurred.
	 * \param userArg           User specific argument.
	 * \return tEplKernel       Returns a tEplKernel error code.
	 */
	tEplKernel ProcessCfmResultEvent(tEplApiEventCfmResult* cfmResult, void GENERIC* userArg);

	/**
	 * \brief   Triggers a Node found signal when an ident response is received from a node.
	 *
	 * \param nodeId    The id of the respective node
	 */
	void TriggerNodeFound(const int nodeId);

	/**
	 * \brief   Triggers a signal when there is a change in the state of the node.
	 *
	 * \param nodeId    The id of the respective node
	 * \param nmtState  The changed state of the respective node
	 */
	void TriggerNodeStateChanged(const int nodeId, tNmtState nmtState);

	/**
	 * \brief   Triggers a signal when there is a change of the state of the stack.
	 *
	 * \param nmtState	The new state to which the local node has changed to.
	 */
	void TriggerLocalNodeStateChanged(tNmtState nmtState);

	/**
	 * \brief   Triggers a signal with the log message
	 *
	 * \param logStr    Log message.
	 */
	void TriggerPrintLog(const QString logStr);

	/**
	 * \brief   Set Default node assignment when CFM manager is not available
	 *
	 * \param logStr    Log message.
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
	 * \param nodeId
	 */
	void SignalNodeFound(const int nodeId);

	/**
	 * \brief   Node state changed
	 *
	 * \param nodeId
	 * \param nmtState
	 */
	void SignalNodeStateChanged(const int nodeId, tNmtState nmtState);

	/**
	 * \brief
	 *
	 * \param nmtState
	 */
	void SignalLocalNodeStateChanged(tNmtState nmtState);

	/**
	 * \brief   Log message of the events occurring
	 *
	 * \param logStr
	 */
	void SignalPrintLog(const QString &logStr);

};

#endif // _OPLK_EVENTHANDLER_H_

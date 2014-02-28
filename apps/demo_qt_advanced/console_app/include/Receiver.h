/**
********************************************************************************
\file   Receiver.h

\brief  openPOWERLINK signal handler.
*******************************************************************************/

#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include <QObject>

#include <oplk/oplk.h>

#include "user/SdoTransferResult.h"

/**
 * \brief Class inherits QObject to receive the signals from the openPOWERLINK stack.
 *
 * The signal handlers can be registered by using the OplkQtApi::Register***() functions.
 */
class Receiver : public QObject
{
	Q_OBJECT

public:
	/**
	 * \brief   Handles the Node State changed singals
	 *
	 * \param[in] nodeId	nodeId of the node which changes the state.
	 * \param[in] nmtState	New state of the node.
	 */
	Q_INVOKABLE void HandleNodeStateChanged(const int nodeId, tNmtState nmtState);

	/**
	 * \brief   Handles the Local Node State changed singals
	 *
	 * \param[in] nmtState	New state of the local node.
	 */
	Q_INVOKABLE void HandleLocalNodeStateChanged(tNmtState nmtState);

	/**
	 * \brief   Handles the log singals
	 *
	 * \param[in] logStr	Detailed log message.
	 */
	Q_INVOKABLE void HandleLogEvent(const QString& logStr);

	/**
	 * \brief   Handles the SDO transfer events.
	 *
	 * \param[in] result	Result of the SDO transfer.
	 */
	Q_INVOKABLE void HandleSdoTransferFinished(const SdoTransferResult result);
};

#endif // _RECEIVER_H_

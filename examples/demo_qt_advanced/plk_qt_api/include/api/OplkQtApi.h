/**
********************************************************************************
\file   OplkQtApi.h

\brief  API to the openPOWERLINK-Stack
*******************************************************************************/

#ifndef _OPLK_QT_API_H_
#define _OPLK_QT_API_H_

#include <QtCore>
#include <string>
#include <QtDebug>
#include <QMetaMethod>
#include <QObject>
#include <QMetaType>

#include "Epl.h"
#include "nmt.h"

#include "api/OplkEventHandler.h"
#include "common/QtApiGlobal.h"

/**
 * \brief API to the openPOWERLINK-Stack
 */
class PLKQTAPI_EXPORT OplkQtApi : QObject
{
	Q_OBJECT

public:
	/**
	 * \brief   Initialize openPOWERLINK-Stack with default init-parameters.
	 *
	 *          Starts the stack with the given nodeId on the given networkInterface.
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

	// TODO(RaM): Link to required signal in doxygen comments
	/**
	 * \brief
	 *
	 * \param[in] receiver			Object to handle the event.
	 * \param[in] receiverFunction	Object-Function to handle the event.
	 * \retval true		Registration successful.
	 *         false	Registration not successful.
	 */
	static bool RegisterNodeFoundEventHandler(const QObject& receiver, const QMetaMethod& receiverFunction);

	/**
	 * \brief
	 *
	 * \param[in] receiver			Object to handle the event.
	 * \param[in] receiverFunction	Object-Function to handle the event.
	 * \retval true		De-registration successful.
	 *         false	De-registration not successful.
	 */
	static bool UnregisterNodeFoundEventHandler(const QObject& receiver, const QMetaMethod& receiverFunction);

	/**
	 * \brief
	 *
	 * \param[in] receiver			Object to handle the event.
	 * \param[in] receiverFunction	Object-Function to handle the event.
	 * \retval true		Registration successful.
	 *         false	Registration not successful.
	 */
	static bool RegisterNodeStateChangedEventHandler(const QObject& receiver, const QMetaMethod& receiverFunction);

	/**
	 * \brief
	 *
	 * \param[in] receiver			Object to handle the event.
	 * \param[in] receiverFunction	Object-Function to handle the event.
	 * \retval true		De-registration successful.
	 *         false	De-registration not successful.
	 */
	static bool UnregisterNodeStateChangedEventHandler(const QObject& receiver, const QMetaMethod& receiverFunction);

	/**
	 * \brief
	 *
	 * \param[in] receiver			Object to handle the event.
	 * \param[in] receiverFunction	Object-Function to handle the event.
	 * \retval true		Registration successful.
	 *         false	Registration not successful.
	 */
	static bool RegisterLocalNodeStateChangedEventHandler(const QObject& receiver, const QMetaMethod& receiverFunction);

	/**
	 * \brief
	 *
	 * \param[in] receiver			Object to handle the event.
	 * \param[in] receiverFunction	Object-Function to handle the event.
	 * \retval true		De-registration successful.
	 *         false	De-registration not successful.
	 */
	static bool UnregisterLocalNodeStateChangedEventHandler(const QObject& receiver, const QMetaMethod& receiverFunction);

	/**
	 * \brief
	 *
	 * \param[in] receiver			Object to handle the event.
	 * \param[in] receiverFunction	Object-Function to handle the event.
	 * \retval true		Registration successful.
	 *         false	Registration not successful.
	 */
	static bool RegisterEventLogger(const QObject& receiver, const QMetaMethod& receiverFunction);

	/**
	 * \brief
	 *
	 * \param[in] receiver			Object to handle the event.
	 * \param[in] receiverFunction	Object-Function to handle the event.
	 * \retval true		De-registration successful.
	 *         false	De-registration not successful.
	 */
	static bool UnregisterEventLogger(const QObject& receiver, const QMetaMethod& receiverFunction);

private:
	static tEplApiInitParam initParam;
	static void SetInitParam();
	OplkQtApi& operator=(const OplkQtApi& api);
	OplkQtApi(const OplkQtApi& api);
	OplkQtApi();
};

#endif //  _OPLK_QT_API_H_

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

#include "Epl.h"
#include "nmt.h"

#include "api/OplkEventHandler.h"
#include "common/global.h"

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
	 * \param nodeId            nodeId to assign to the local stack-instance.
	 * \param networkInterface  network interface to run the openPOWERLINK-Stack on.
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

private:
	static tEplApiInitParam initParam;
	static void SetInitParam();
	OplkQtApi& operator=(const OplkQtApi& api);
	OplkQtApi(const OplkQtApi& api);
	OplkQtApi();

};

#endif //  _OPLK_QT_API_H_

/**
********************************************************************************
\file   ConsoleReader.h

\brief  Console input reader thread.
*******************************************************************************/

#ifndef _CONSOLEREADER_H_
#define _CONSOLEREADER_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <QThread>

#include "console/console.h"
#include "api/OplkQtApi.h"

#include "../include/Receiver.h"

/**
 * \brief Class to read the inputs from the console window.
 */
class ConsoleReader : public QThread
{
public:
	ConsoleReader();
	void run();
	DWORD GetSdoReadData();
	DWORD GetSdoWriteData();

private:
	DWORD *sdoReadData;
	DWORD *sdoWriteData;
	SdoTransferJob *sdoReadTransferJob;
	SdoTransferJob *sdoWriteTransferJob;
	Receiver receiver;
};

#endif // _CONSOLEREADER_H_

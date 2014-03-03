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

#include "api/OplkQtApi.h"
#include "user/SdoTransferJob.h"

#include "../include/Receiver.h"

/**
 * \brief Class to read the inputs from the console window.
 */
class ConsoleReader : public QThread
{
public:
	ConsoleReader();
	void run();
	const ULONG GetSdoReadData() const;
	const ULONG GetSdoWriteData() const;

private:
	ULONG sdoReadData;
	ULONG sdoWriteData;
	SdoTransferJob *sdoReadTransferJob;
	SdoTransferJob *sdoWriteTransferJob;
	Receiver receiver;

	const UINT nodeId;
	std::string xapFileName;

	const std::string inputChannelName;
	const std::string outputChannelName;

	const std::string sdoReceiverFunction;

	std::string devName;


};

#endif // _CONSOLEREADER_H_

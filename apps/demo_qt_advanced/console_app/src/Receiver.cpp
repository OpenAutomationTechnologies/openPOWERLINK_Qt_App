/**
********************************************************************************
\file   Receiver.cpp

\brief  Signal receiver implementation
*******************************************************************************/

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <oplk/debugstr.h>
#include "../include/Receiver.h"

void Receiver::HandleNodeStateChanged(const int nodeId, tNmtState nmtState)
{
	qDebug("Node %d changed state to %s.", nodeId, debugstr_getNmtStateStr(nmtState));
}

void Receiver::HandleLocalNodeStateChanged(tNmtState nmtState)
{
	qDebug("Local Node changed state to %s.", debugstr_getNmtStateStr(nmtState));
}

void Receiver::HandleLogEvent(const QString& logStr)
{
	qDebug("%s", logStr.toStdString().c_str());
}

void Receiver::HandleSdoTransferFinished(const SdoTransferResult result)
{
	qDebug("SdoTransferFinished Abort: %x, NodeId: %x, Index: %x, SubIndex:%x state:%x",
		   result.GetAbortCode(), result.GetNodeId(),
		   result.GetIndex(), result.GetSubIndex(), result.GetSdoComConState());
	if (result.GetAbortCode() != 0)
	{

	}
}

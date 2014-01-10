/**
********************************************************************************
\file   Receiver.cpp

\brief  Signal receiver implementation
*******************************************************************************/

#include "../include/Receiver.h"

void Receiver::HandleNodeStateChanged(const int nodeId, tNmtState nmtState)
{
	qDebug("Receiver %#x: Node %d changed state to %d.", this, nodeId, nmtState);
}

void Receiver::HandleLocalNodeStateChanged(tNmtState nmtState)
{
	qDebug("Receiver %#x: Local Node changed state to %x.", this, nmtState);
}

void Receiver::HandleLogEvent(const QString& logStr)
{
	qDebug("Receiver %#x: %s", this, logStr.toStdString().c_str());
}

void Receiver::HandleSdoTransferFinished(const SdoTransferResult result)
{
	qDebug("Receiver %#x: SdoTransferFinished Abort: %x, NodeId: %x, Index: %x, SubIndex:%x state:%x",
		   this, result.GetAbortCode(), result.GetNodeId(),
		   result.GetIndex(), result.GetSubIndex(), result.GetSdoComConState());
	if (0 != result.GetAbortCode())
	{
	
	}
}

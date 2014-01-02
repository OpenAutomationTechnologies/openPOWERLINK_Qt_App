/**
********************************************************************************
\file   ReceiverContext.cpp

\brief  Implementation of ReceiverContext class
*******************************************************************************/

#include "api/ReceiverContext.h"

ReceiverContext::ReceiverContext(const QObject* receiver,
		const QMetaMethod* receiverFunction) :
		receiver(receiver),
		receiverFunction(receiverFunction)
{

}

const QObject* ReceiverContext::GetReceiver() const
{
	return this->receiver;
}

const QMetaMethod* ReceiverContext::GetReceiverFunction() const
{
	return this->receiverFunction;
}

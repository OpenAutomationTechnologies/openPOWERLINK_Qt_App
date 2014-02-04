/**
********************************************************************************
\file   ReceiverContext.h

\brief  The context for the receiver implementation
*******************************************************************************/

#ifndef _RECEIVERCONTEXT_H_
#define _RECEIVERCONTEXT_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <QObject>
#include <QMetaMethod>

/**
 * \brief The context for the receiver implementation
 */
class ReceiverContext
{
public:
	/**
	 * \brief	Parameterized constructor with receiver and receiver function.
	 *
	 * \param[in] receiver			Receiver class object.
	 * \param[in] receiverFunction	Receiver function pointer.
	 */
	ReceiverContext(const QObject* receiver,
		const QMetaMethod* receiverFunction);

	const QObject* GetReceiver() const;

	const QMetaMethod* GetReceiverFunction() const;

private:
	const QObject* receiver;
	const QMetaMethod* receiverFunction;
};

#endif // _RECEIVERCONTEXT_H_

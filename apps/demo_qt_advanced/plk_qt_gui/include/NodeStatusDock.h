#ifndef NODESTATUSDOCK_H
#define NODESTATUSDOCK_H

#include "ui_NodeStatusDock.h"
#include "Node.h"

class NodeStatusDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit NodeStatusDock(QWidget *parent = 0);
	// NodeStatusDock& GetInstance();
	QStringList NodeStatusDock::GetAvailableCnList();
	/**
	 * \brief   Handles the Node State changed singals
	 *
	 * \param[in] nodeId	nodeId of the node which changes the state.
	 * \param[in] nmtState	New state of the node.
	 */
	Q_INVOKABLE void HandleNodeStateChanged(const int nodeId/*, tNmtState nmtState*/);
	Q_INVOKABLE void HandleNodeFound(const int nodeId);

private:
	Ui::NodeStatusDock ui;
	Node **nodelist;
	Q_DISABLE_COPY(NodeStatusDock)
};

#endif // NODESTATUSDOCK_H

#ifndef _NODESTATUSDOCK_H_
#define _NODESTATUSDOCK_H_

#include <QList>

#include "ui_NodeStatusDock.h"
#include "NodeUi.h"

class NodeStatusDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit NodeStatusDock(QWidget *parent = 0);
	// NodeStatusDock& GetInstance();
	QStringList GetAvailableCnList();
	/**
	 * \brief   Handles the Node State changed singals
	 *
	 * \param[in] nodeId	nodeId of the node which changes the state.
	 * \param[in] nmtState	New state of the node.
	 */
	Q_INVOKABLE void HandleNodeStateChanged(const int nodeId, tNmtState nmtState);
	Q_INVOKABLE void HandleNodeFound(const int nodeId);
	Q_INVOKABLE void HandleNodeStateChanged(tNmtState nmtState);
private:
	Ui::NodeStatusDock ui;
	// TODO replace with QList
	// NodeUi **nodelist;
	QList<NodeUi*> nodelists;
	// May be we can use QMap
	// QMap<const uint, NodeUi*> nodelist;
	// Q_DISABLE_COPY(NodeStatusDock)
};

#endif // _NODESTATUSDOCK_H_

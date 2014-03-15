#ifndef _NODE_UI_H_
#define _NODE_UI_H_

#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>

#include <oplk/nmt.h>

class NodeUi : public QFrame
{
	Q_OBJECT

public:
	explicit NodeUi(const uint nodeId, QWidget *parent = 0);
	Q_INVOKABLE void HandleNodeStateChanged(tNmtState nmtState);
	unsigned int GetNodeId() const;

private:
	QHBoxLayout *nodeLayout;
	QLabel *name;
	QLabel *statusImage;
	QPixmap statusPixmap;
	unsigned int nodeId;
	// Q_DISABLE_COPY(Node)
};

#endif // _NODE_UI_H_

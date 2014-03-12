#ifndef _NODE_UI_H_
#define _NODE_UI_H_

#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>

class NodeUi : public QFrame
{
	Q_OBJECT

public:
	explicit NodeUi(const QString& nodeName, QWidget *parent = 0);
	void SetNodeStatus(int state);

private:
	QHBoxLayout *nodeLayout;
	QLabel *name;
	QLabel *statusImage;
	QPixmap statusPixmap;
	QString statusStr;
	// Q_DISABLE_COPY(Node)
};

#endif // _NODE_UI_H_

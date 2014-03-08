#ifndef NODE_H
#define NODE_H

#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>

class Node : public QFrame
{
	Q_OBJECT

public:
	explicit Node(const QString& nodeName, QWidget *parent = 0);
	void SetNodeStatus(int state);

private:
	QHBoxLayout *nodeLayout;
	QLabel *name;
	QLabel *statusImage;
	QPixmap statusPixmap;
	QString statusStr;
};

#endif // NODE_H

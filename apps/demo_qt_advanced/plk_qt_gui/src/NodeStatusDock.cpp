#include "NodeStatusDock.h"

const unsigned int kMaxCnNodes = 239;

NodeStatusDock::NodeStatusDock(QWidget *parent) :
	QDockWidget(parent)
{
	this->ui.setupUi(this);

	this->nodelist = new Node*[kMaxCnNodes + 1];
	for (unsigned int i = 1; i <= 10; ++i)
	{
		this->nodelist[i] = new Node(QString("CN %1") .arg(i));
		this->ui.verticalLayout->addWidget(this->nodelist[i]);
		this->nodelist[i]->setToolTip(QString("CN %1") .arg(i));
		// this->nodelist[i]->hide();
	}
}

void NodeStatusDock::HandleNodeStateChanged(const int nodeId/*, tNmtState nmtState*/)
{
	if (!(this->nodelist[nodeId]))
	{
		this->nodelist[nodeId]->setToolTip(QString("CN %1") .arg(nodeId));
		this->nodelist[nodeId]->SetNodeStatus(0/*nmtState*/);
	}
}

void NodeStatusDock::HandleNodeFound(const int nodeId)
{
	// The memory is created only if the node found signal is detected.
	if ((nodeId > 0) && (nodeId <= kMaxCnNodes))
	{
		if (!(this->nodelist[nodeId]))
		{
			this->nodelist[nodeId] = new Node(QString("CN %1") .arg(nodeId));
			this->ui.verticalLayout->addWidget(this->nodelist[nodeId]);
			this->nodelist[nodeId]->setToolTip(QString("CN %1") .arg(nodeId));
		}
	}
}

QStringList NodeStatusDock::GetAvailableCnList()
{
	QStringList cnList;
	for (uint i = 1; i <= kMaxCnNodes; ++i)
	{
		if (!(this->nodelist[i]))
		{
			cnList << QString("0x%1").arg(i, 2, 16);
		}
	}
	return cnList;
}

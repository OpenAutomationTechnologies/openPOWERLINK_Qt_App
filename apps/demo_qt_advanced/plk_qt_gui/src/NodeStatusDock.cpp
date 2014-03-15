#include "NodeStatusDock.h"
#include "api/OplkQtApi.h"

#include <oplk/debugstr.h>

const uint kMaxCnNodes = 239;

NodeStatusDock::NodeStatusDock(QWidget *parent) :
	QDockWidget(parent)
{
	this->ui.setupUi(this);

//	this->nodelist = new NodeUi*[kMaxCnNodes + 1];
//	for (uint i = 1; i <= kMaxCnNodes; ++i)
//	{
//		this->nodelist[i] = new NodeUi(i);
//		this->ui.verticalLayout->addWidget(this->nodelist[i]);
//		//this->nodelist[i]->setToolTip(QString("CN %1") .arg(i));
//		this->nodelist[i]->hide();
//	}

	NodeUi *nodeUi = NULL;
	for (uint i = 0; i < kMaxCnNodes; ++i)
	{
		nodeUi = new NodeUi(i + 1);
		nodeUi->hide();
		this->nodelists.push_back(nodeUi);
		this->ui.verticalLayout->addWidget(nodeUi);
	}

	// TODO Handle return values
	int index = this->metaObject()->indexOfMethod(
					QMetaObject::normalizedSignature(
					"HandleNodeStateChanged(const int, tNmtState)").constData());
	// TODO Handle return values
	bool ret = OplkQtApi::RegisterNodeStateChangedEventHandler(*(this),
							this->metaObject()->method(index));


	// TODO Handle return values
	index = this->metaObject()->indexOfMethod(
					QMetaObject::normalizedSignature(
					"HandleNodeFound(const int)").constData());
	// TODO Handle return values
	ret = OplkQtApi::RegisterNodeFoundEventHandler(*(this),
							this->metaObject()->method(index));

// TODO Handle return values
	index = this->metaObject()->indexOfMethod(
					QMetaObject::normalizedSignature(
						"HandleNodeStateChanged(tNmtState)").constData());
// TODO Handle return values
	ret = OplkQtApi::RegisterLocalNodeStateChangedEventHandler(*(this),
							this->metaObject()->method(index));

}

void NodeStatusDock::HandleNodeStateChanged(tNmtState nmtState)
{
	// Change all CN states for Stack shutdown / MN dead.
	if ((nmtState == kNmtGsOff) || (nmtState < kNmtMsOperational))
	{
		for (QList<NodeUi*>::iterator it = this->nodelists.begin();
			 it != this->nodelists.end(); ++it)
		{
			this->HandleNodeStateChanged((*it)->GetNodeId(), kNmtCsNotActive);
		}
	}
}

void NodeStatusDock::HandleNodeStateChanged(const int nodeId, tNmtState nmtState)
{
	if ((nodeId > 0) && (nodeId <= kMaxCnNodes))
	{
		if ((this->nodelists.at(nodeId - 1)))
		{
			this->nodelists.at(nodeId - 1)->HandleNodeStateChanged(nmtState);
			// Hide CN for Loss of PRes
			if (nmtState == kNmtCsNotActive)
			{
				this->nodelists.at(nodeId - 1)->hide();
			}
		}
	}
}

void NodeStatusDock::HandleNodeFound(const int nodeId)
{
	if ((nodeId > 0) && (nodeId <= kMaxCnNodes))
	{
		if (this->nodelists.at(nodeId - 1))
		{
			this->nodelists.at(nodeId - 1)->show();
		}
	}
}

QStringList NodeStatusDock::GetAvailableCnList()
{
	QStringList cnList;
//	for (uint i = 1; i <= kMaxCnNodes; ++i)
//	{
//		if ((this->nodelist[i]))
//		{
//			cnList << QString("0x%1").arg(i, 2, 16);
//		}
//	}
	return cnList;
}

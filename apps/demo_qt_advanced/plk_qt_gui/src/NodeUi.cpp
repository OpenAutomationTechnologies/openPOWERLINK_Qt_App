#include "NodeUi.h"
#include <QPainter>
#include <QPen>
#include <QBrush>

#include <oplk/debugstr.h>

NodeUi::NodeUi(const uint nodeId, QWidget *parent) :
	QFrame(parent),
	nodeId(nodeId),
	nodeLayout(new QHBoxLayout(this)),
	name(new QLabel()),
	statusImage(new QLabel()),
	statusPixmap(QPixmap(QSize(27, 27)))
{
/// Node name
	if (this->nodeId == 240)
		this->name->setText(QString("MN - %1 : ").arg(this->nodeId));
	else
		this->name->setText(QString("CN - %1").arg(this->nodeId));

	this->name->setFont(QFont("Arial", 15, QFont::Bold));
	this->nodeLayout->addWidget(this->name);

/// Node name
	this->statusPixmap.fill(Qt::transparent);

	QPainter painter(&(this->statusPixmap));
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(QPen(Qt::transparent));
	painter.setBrush(QBrush(Qt::red));
	painter.drawEllipse(0, 0, 25, 25);

	this->statusImage->setPixmap(this->statusPixmap);
	this->nodeLayout->addWidget(this->statusImage);

	this->setToolTip(this->name->text());

	this->nodeLayout->update();
}
unsigned int NodeUi::GetNodeId() const
{
	return this->nodeId;
}

void NodeUi::HandleNodeStateChanged(tNmtState nmtState)
{
	this->setToolTip(QString("%1 %2") .arg(this->name->text())
			.arg(debugstr_getNmtStateStr(nmtState)));

	QPainter painter(&(this->statusPixmap));
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(QPen(Qt::transparent));

	// can also use QBrush(const QGradient & gradient)
	switch (nmtState)
	{
		case kNmtMsNotActive:
		case kNmtCsNotActive:
		{
			painter.setBrush(QBrush(Qt::gray));
			break;
		}
		case kNmtMsPreOperational1:
		case kNmtMsPreOperational2:
		case kNmtCsPreOperational1:
		case kNmtCsPreOperational2:
		{
			painter.setBrush(QBrush(Qt::yellow));
			break;
		}
		case kNmtGsOff:
		case kNmtCsStopped:
		{
			painter.setBrush(QBrush(Qt::red));
			break;
		}
		case kNmtMsReadyToOperate:
		case kNmtCsReadyToOperate:
		{
			painter.setBrush(QColor(255, 111, 0));
			//painter.setBrush(QBrush(Qt::magenta));
			break;
		}
		case kNmtMsOperational:
		case kNmtCsOperational:
		{
			painter.setBrush(QBrush(Qt::green));
			break;
		}
		case kNmtMsBasicEthernet:
		case kNmtCsBasicEthernet:
		{
			painter.setBrush(QBrush(Qt::blue));
			break;
		}
		case kNmtGsInitialising:
		case kNmtGsResetApplication:
		case kNmtGsResetCommunication:
		case kNmtGsResetConfiguration:
		default:
			painter.setBrush(QBrush(Qt::white));
			break;
	}
	painter.drawEllipse(0, 0, 25, 25);
	this->statusImage->setPixmap(this->statusPixmap);
	this->nodeLayout->update();
}

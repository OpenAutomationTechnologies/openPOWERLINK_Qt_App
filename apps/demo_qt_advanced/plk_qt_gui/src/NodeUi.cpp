#include "NodeUi.h"
#include <QPainter>
#include <QPen>
#include <QBrush>

NodeUi::NodeUi(const QString& nodeName, QWidget *parent) :
	QFrame(parent),
	name(new QLabel(nodeName)),
	statusImage(new QLabel()),
	statusPixmap(QPixmap(QSize(30, 30)))
{
	this->setObjectName(QStringLiteral("Node"));

//	this->setFrameShape(QFrame::StyledPanel);
//	this->setFrameShadow(QFrame::Raised);
	this->nodeLayout = new QHBoxLayout(this);
	this->nodeLayout->setObjectName(QStringLiteral("verticalLayout"));

	QMetaObject::connectSlotsByName(this);

	this->name->setFont(QFont("Arial", 15, QFont::Bold));
	this->nodeLayout->addWidget(name);

	//this->statusImage = new QLabel();
	//statusImage->setPalette(QPalette(Qt::green));

//	QPixmap pixmap( 200, 190 );
//	pixmap.fill( Qt::white );

	//this->statusPixmap(QSize(28, 28));
	this->statusPixmap.fill(Qt::transparent);

	QPainter p(&(this->statusPixmap));
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setPen(QPen(Qt::transparent));
	p.setBrush(QBrush(Qt::red));
	p.drawEllipse(0, 0, 25, 25);
	this->statusImage->setPixmap(this->statusPixmap);
	this->nodeLayout->addWidget(this->statusImage);

	this->nodeLayout->update();
}

void NodeUi::SetNodeStatus(int state) // param tNMTState
{
	QPainter p(&(this->statusPixmap));
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setPen(QPen(Qt::transparent));

	// can also use QBrush(const QGradient & gradient)
//	switch (state)
//	{
//	case kNmtCsNotActive:
//	{
//		p.setBrush(QBrush(Qt::lightGray));
//		break;
//	}
//	case kNmtCsPreOperational1:
//	case kNmtCsPreOperational2:
//	{
//		p.setBrush(QBrush(Qt::yellow));
//		break;
//	}
//	case kNmtCsStopped:
//	{
//		p.setBrush(QBrush(Qt::red));
//		break;
//	}
//	case kNmtCsReadyToOperate:
//	{
//		p.setBrush(QBrush(Qt::magenta));
//		break;
//	}
//	case kNmtCsOperational:
//	{
//		p.setBrush(QBrush(Qt::green));
//		break;
//	}
//	case kNmtCsBasicEthernet:
//	{
//		p.setBrush(QBrush(Qt::blue));
//		break;
//	}
//	default:
//		p.setBrush(QBrush(Qt::white));
//		break;
//	}
//	p.drawEllipse(0, 0, 25, 25);
//	this->statusImage->setPixmap(this->statusPixmap);

//	kNmtGsOff
//    kNmtGsInitialising
//    kNmtGsResetApplication
//    kNmtGsResetCommunication
//    kNmtGsResetConfiguration

	switch (state)
	{
		case 0:
		{
			p.setBrush(QBrush(Qt::cyan));
			p.drawEllipse(0, 0, 25, 25);
			this->statusImage->setPixmap(this->statusPixmap);
			break;
		}
		case 1:
			break;
		case 2:
			break;
		default:
			break;
	}
	this->nodeLayout->update();
}

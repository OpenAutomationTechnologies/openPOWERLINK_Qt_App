#include "Channel.h"
// true output output
Channel::Channel(bool direction, QString channelName, QWidget *parent) :
	QWidget(parent)
{
	ui.setupUi(this);
	this->ui.channelName->setText(channelName);

	if (direction)
	{
		this->ui.force->hide();
		this->ui.forceValue->hide();
	}
}

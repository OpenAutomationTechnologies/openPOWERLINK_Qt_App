#ifndef CHANNEL_H
#define CHANNEL_H

#include "ui_Channel.h"

class Channel : public QWidget
{
	Q_OBJECT

public:
	// bool has to be Pi direction
	explicit Channel(bool direction, QString channelName, QWidget *parent = 0);

private:
	Ui::Channel ui;
};

#endif // PROCESSIMAGEVARIABLE_H

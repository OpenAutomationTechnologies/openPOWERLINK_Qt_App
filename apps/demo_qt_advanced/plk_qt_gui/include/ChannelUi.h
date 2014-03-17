#ifndef _UI_CHANNEL_H_
#define _UI_CHANNEL_H_

#include "ui_ChannelUi.h"
#include "user/processimage/Channel.h"
#include "user/processimage/ProcessImageIn.h"
#include "user/processimage/ProcessImageOut.h"

class ChannelUi : public QWidget
{
	Q_OBJECT

public:
	// bool has to be Pi direction
	explicit ChannelUi(Channel channel, QWidget *parent = 0);

	~ChannelUi();

	void UpdateSelectCheckBox(Qt::CheckState forceState);

	Qt::CheckState GetSelectCheckBoxState();

	void SetCurrentValue(QString setStr);

	QString GetCurrentValue();

	QString GetForceValue();

	void UpdateForceCheckBox(Qt::CheckState forceState);

	Qt::CheckState GetForceCheckBoxState();

	void UpdateInputChannelCurrentValue(ProcessImageIn *in);
	void UpdateOutputChannelCurrentValue(ProcessImageOut *out);

private slots:
	void on_check_stateChanged(int arg1);

	void on_force_stateChanged(int arg1);

private:
	Ui::ChannelFrame ui;
	// TODO change to ptr. TODO ptr in channelMap in api-lib
	Channel channel;

	//Q_DISABLE_COPY(Channel)
};

#endif // _UI_CHANNEL_H_

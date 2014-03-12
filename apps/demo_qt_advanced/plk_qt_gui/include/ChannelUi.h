#ifndef _UI_CHANNEL_H_
#define _UI_CHANNEL_H_

#include "ui_ChannelUi.h"

class ChannelUi : public QWidget
{
	Q_OBJECT

public:
	// bool has to be Pi direction
	explicit ChannelUi(bool direction, QString channelName, QWidget *parent = 0);

	void UpdateSelectCheckBox(Qt::CheckState forceState);

	Qt::CheckState GetSelectCheckBoxState();

	void SetCurrentValue(QString setStr);

	QString GetCurrentValue();

	QString GetForceValue();

	void UpdateForceCheckBox(Qt::CheckState forceState);

	Qt::CheckState GetForceCheckBoxState();

	void UpdateCurrentValue();

private slots:
	void on_check_stateChanged(int arg1);

	void on_force_stateChanged(int arg1);

private:
	Ui::ChannelFrame ui;
	//Q_DISABLE_COPY(Channel)
};

#endif // _UI_CHANNEL_H_

#include "ChannelUi.h"
// direction = true for output
// todo change as ChannelUI(Channel, QWidget *parent)
ChannelUi::ChannelUi(bool direction, QString channelName, QWidget *parent) :
	QWidget(parent)
{
	ui.setupUi(this);
	this->ui.channelName->setText(channelName);

	// Hide force check box and forcevalue text box for output PI
	if (direction)
	{
		this->ui.force->hide();
		this->ui.forceValue->hide();
	}
	// this->
}
ChannelUi::~ChannelUi()
{
	delete this->ui.channelName;
	delete this->ui.check;
	delete this->ui.currentValue;
	delete this->ui.force;
	delete this->ui.horizontalLayout;
}

void ChannelUi::on_check_stateChanged(int arg1)
{

}

void ChannelUi::on_force_stateChanged(int arg1)
{

}

void ChannelUi::UpdateSelectCheckBox(Qt::CheckState forceState)
{
	this->ui.check->setChecked(forceState);
}

Qt::CheckState ChannelUi::GetSelectCheckBoxState()
{
	return this->ui.check->checkState();
}

void ChannelUi::SetCurrentValue(QString setStr)
{
	this->ui.currentValue->setText(setStr);
}

QString ChannelUi::GetCurrentValue()
{
	return this->ui.currentValue->text();
}

QString ChannelUi::GetForceValue()
{
	return this->ui.forceValue->text();
}

void ChannelUi::UpdateForceCheckBox(Qt::CheckState forceState)
{
	this->ui.force->setChecked(forceState);
}

Qt::CheckState ChannelUi::GetForceCheckBoxState()
{
	return this->ui.force->checkState();
}

void ChannelUi::UpdateCurrentValue()
{
	if (1) // Output
	{
		// piout.GetRawData();
		this->SetCurrentValue("str");
	}
	else // Input
	{
		// piin.GetRawData();
		this->SetCurrentValue("str");
		if (this->ui.force->checkState() == Qt::Checked)
		{
			//piin.setRawData(this->ui.forceValue);
		}
	}
}

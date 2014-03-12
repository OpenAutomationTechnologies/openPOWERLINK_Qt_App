#include "ProcessImageVariables.h"
#include <QCheckBox>
#include <QStringList>

const uint processImageInSize = 20;
const uint processImageOutSize = 20;

ProcessImageVariables::ProcessImageVariables(QWidget *parent) :
	QFrame(parent)
{
	ui.setupUi(this);
	this->PrepareInputRows();
	this->PrepareOutputRows();
}

ProcessImageVariables::~ProcessImageVariables()
{
//	qDeleteAll(this->inputChannels.constBegin(), this->inputChannels.constEnd());
//	qDeleteAll(this->outputChannels.constBegin(), this->outputChannels.constEnd());
}

void ProcessImageVariables::PrepareInputRows()
{
	// TODO Loop through processImage In from the QtProcessImageParser and add the channel.
	for (uint i = 0; i < processImageInSize; ++i)
	{
		this->inputChannels.push_back(new ChannelUi(false, "true, obj.get_ChannelName()" ));
		this->ui.inputProcessImage->addWidget(this->inputChannels.at(i));
	}
}

void ProcessImageVariables::PrepareOutputRows()
{
	for (uint i = 0; i < processImageOutSize; ++i)
	{
		this->outputChannels.push_back(new ChannelUi(true, "true, obj.get_ChannelName()" ));
		this->ui.outputProcessImage->addWidget(this->outputChannels.at(i));
	}
}

void ProcessImageVariables::on_inputCheckAll_stateChanged(int checkedState)
{
	if (checkedState != Qt::PartiallyChecked)
	{
		for (QList<ChannelUi*>::iterator i = this->inputChannels.begin();
			 i != this->inputChannels.end(); ++i)
		{
			if (*i)
			{
				(*i)->UpdateSelectCheckBox((Qt::CheckState) checkedState);
			}
		}
	}
}

void ProcessImageVariables::on_outputCheckAll_stateChanged(int checkedState)
{
	if (checkedState != Qt::PartiallyChecked)
	{
		for (QList<ChannelUi*>::iterator i = this->outputChannels.begin();
			 i != this->outputChannels.end(); ++i)
		{
			if (*i)
			{
				(*i)->UpdateSelectCheckBox((Qt::CheckState) checkedState);
			}
		}
	}
}

void ProcessImageVariables::on_inputForceAll_stateChanged(int checkedState)
{
	if (checkedState != Qt::PartiallyChecked)
	{
		for (QList<ChannelUi*>::iterator i = this->inputChannels.begin();
			 i != this->inputChannels.end(); ++i)
		{
			if (*i)
			{
				(*i)->UpdateForceCheckBox((Qt::CheckState) checkedState);
			}
		}
	}
}

void ProcessImageVariables::on_inputHideCheckedBtn_clicked()
{
	for (QList<ChannelUi*>::iterator i = this->inputChannels.begin();
		 i != this->inputChannels.end(); ++i)
	{
		if (*i)
		{
			if ((*i)->GetSelectCheckBoxState() == Qt::Checked)
			{
				(*i)->hide();
			}
		}
	}
}

void ProcessImageVariables::on_inputShowAllBtn_clicked()
{
	for (QList<ChannelUi*>::iterator i = this->inputChannels.begin();
		 i != this->inputChannels.end(); ++i)
	{
		if (*i)
		{
			(*i)->show();
		}
	}
}

void ProcessImageVariables::on_outHideCheckedBtn_clicked()
{
	for (QList<ChannelUi*>::iterator i = this->outputChannels.begin();
		 i != this->outputChannels.end(); ++i)
	{
		if (*i)
		{
			if ((*i)->GetSelectCheckBoxState() == Qt::Checked)
			{
				(*i)->hide();
			}
		}
	}
}

void ProcessImageVariables::on_outShowAllBtn_clicked()
{
	for (QList<ChannelUi*>::iterator i = this->outputChannels.begin();
		 i != this->outputChannels.end(); ++i)
	{
		if (*i)
		{
			(*i)->show();
		}
	}
}

Qt::CheckState ProcessImageVariables::GetInputSelectAllCheckBoxState() const
{
	return this->ui.inputCheckAll->checkState();
}

Qt::CheckState ProcessImageVariables::GetOutputSelectAllCheckBoxState() const
{
	return this->ui.outputCheckAll->checkState();
}

void ProcessImageVariables::UpdateInputs()
{
	for (QList<ChannelUi*>::iterator i = this->inputChannels.begin();
		 i != this->inputChannels.end(); ++i)
	{
		if (*i)
		{
			(*i)->UpdateCurrentValue();
		}
	}
}

void ProcessImageVariables::UpdateOutputs()
{
	for (QList<ChannelUi*>::iterator i = this->outputChannels.begin();
		 i != this->outputChannels.end(); ++i)
	{
		if (*i)
		{
			(*i)->UpdateCurrentValue();
		}
	}
}

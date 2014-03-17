#include "ProcessImageVariables.h"

ProcessImageVariables::ProcessImageVariables(ProcessImageIn &in, ProcessImageOut &out, QWidget *parent) :
	QFrame(parent),
	inPi(in),
	outPi(out)
{
	this->ui.setupUi(this);
	this->PrepareInputRows();
	this->PrepareOutputRows();
}

ProcessImageVariables::~ProcessImageVariables()
{
	for (QList<ChannelUi*>::iterator channel = this->inputChannels.begin();
		 channel != this->inputChannels.end(); ++channel)
	{
		if (*channel)
		{
			delete (*channel);
		}
	}

	for (QList<ChannelUi*>::iterator channel = this->outputChannels.begin();
		 channel != this->outputChannels.end(); ++channel)
	{
		if (*channel)
		{
			delete (*channel);
		}
	}
}

void ProcessImageVariables::PrepareInputRows()
{
	// TODO Loop through processImage In from the QtProcessImageParser and add the channel.
	ChannelUi *channel = NULL;
	for (std::map<std::string, Channel>::const_iterator it = this->inPi.cbegin();
		 it!=this->inPi.cend(); ++it)
	{
		// qDebug(qPrintable(QString::fromStdString(it->first)));
		channel = new ChannelUi(it->second);
		this->inputChannels.push_back(channel);
		this->ui.inputProcessImage->addWidget(channel);
	}
}

void ProcessImageVariables::PrepareOutputRows()
{
	ChannelUi *channel = NULL;
	for (std::map<std::string, Channel>::const_iterator it = this->outPi.cbegin();
		 it!=this->outPi.cend(); ++it)
	{
		// qDebug(qPrintable(QString::fromStdString(it->first)));
		channel = new ChannelUi(it->second);
		this->outputChannels.push_back(channel);
		this->ui.outputProcessImage->addWidget(channel);
	}
}

void ProcessImageVariables::on_inputCheckAll_stateChanged(int checkedState)
{
	if (checkedState != Qt::PartiallyChecked)
	{
		for (QList<ChannelUi*>::iterator channel = this->inputChannels.begin();
			 channel != this->inputChannels.end(); ++channel)
		{
			if (*channel)
			{
				(*channel)->UpdateSelectCheckBox((Qt::CheckState) checkedState);
			}
		}
	}
}

void ProcessImageVariables::on_outputCheckAll_stateChanged(int checkedState)
{
	if (checkedState != Qt::PartiallyChecked)
	{
		for (QList<ChannelUi*>::iterator channel = this->outputChannels.begin();
			 channel != this->outputChannels.end(); ++channel)
		{
			if (*channel)
			{
				(*channel)->UpdateSelectCheckBox((Qt::CheckState) checkedState);
			}
		}
	}
}

void ProcessImageVariables::on_inputForceAll_stateChanged(int checkedState)
{
	if (checkedState != Qt::PartiallyChecked)
	{
		for (QList<ChannelUi*>::iterator channel = this->inputChannels.begin();
			 channel != this->inputChannels.end(); ++channel)
		{
			if (*channel)
			{
				(*channel)->UpdateForceCheckBox((Qt::CheckState) checkedState);
			}
		}
	}
}

void ProcessImageVariables::on_inputHideCheckedBtn_clicked()
{
	for (QList<ChannelUi*>::iterator channel = this->inputChannels.begin();
		 channel != this->inputChannels.end(); ++channel)
	{
		if (*channel)
		{
			if ((*channel)->GetSelectCheckBoxState() == Qt::Checked)
			{
				(*channel)->hide();
			}
		}
	}
}

void ProcessImageVariables::on_inputShowAllBtn_clicked()
{
	for (QList<ChannelUi*>::iterator channel = this->inputChannels.begin();
		 channel != this->inputChannels.end(); ++channel)
	{
		if (*channel)
		{
			(*channel)->show();
		}
	}
}

void ProcessImageVariables::on_outHideCheckedBtn_clicked()
{
	for (QList<ChannelUi*>::iterator channel = this->outputChannels.begin();
		 channel != this->outputChannels.end(); ++channel)
	{
		if (*channel)
		{
			if ((*channel)->GetSelectCheckBoxState() == Qt::Checked)
			{
				(*channel)->hide();
			}
		}
	}
}

void ProcessImageVariables::on_outShowAllBtn_clicked()
{
	for (QList<ChannelUi*>::iterator channel = this->outputChannels.begin();
		 channel != this->outputChannels.end(); ++channel)
	{
		if (*channel)
		{
			(*channel)->show();
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
	for (QList<ChannelUi*>::iterator channel = this->inputChannels.begin();
		 channel != this->inputChannels.end(); ++channel)
	{
		if (*channel)
		{
			(*channel)->UpdateInputChannelCurrentValue(&(this->inPi));
		}
	}
}

void ProcessImageVariables::UpdateOutputs()
{
	for (QList<ChannelUi*>::iterator channel = this->outputChannels.begin();
		 channel != this->outputChannels.end(); ++channel)
	{
		if (*channel)
		{
			(*channel)->UpdateOutputChannelCurrentValue(&(this->outPi));
		}
	}
}


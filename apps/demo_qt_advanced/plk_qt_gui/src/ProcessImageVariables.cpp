#include "ProcessImageVariables.h"
#include <QCheckBox>
#include <QStringList>
const unsigned int processImageInSize = 10;
const unsigned int processImageOutSize = 10;
ProcessImageVariables::ProcessImageVariables(QWidget *parent) :
	QFrame(parent)
{
	ui.setupUi(this);
	this->inputVarList = new Channel*[processImageInSize + 1];
	for (int i = 0; i < processImageInSize; ++i)
	{
		this->inputVarList[i] = new Channel(false, "true, obj.get_ChannelName()" );
		this->ui.inputProcessImage->addWidget(this->inputVarList[i]);
	}

	this->outputVarList = new Channel*[processImageOutSize + 1];
	for (int i = 0; i < processImageInSize; ++i)
	{
		this->outputVarList[i] = new Channel(true, "true, obj.get_ChannelName()" );
		this->ui.outputProcessImage->addWidget(this->outputVarList[i]);
	}
}

void ProcessImageVariables::on_inputCheckAll_stateChanged(int arg1)
{

}

void ProcessImageVariables::on_outputCheck_stateChanged(int arg1)
{

}

void ProcessImageVariables::on_inputForceAll_stateChanged(int arg1)
{

}

void ProcessImageVariables::on_inputHideCheckedBtn_clicked()
{

}

void ProcessImageVariables::on_inputShowAllBtn_clicked()
{

}

void ProcessImageVariables::on_outHideCheckedBtn_clicked()
{

}

void ProcessImageVariables::on_outShowAllBtn_clicked()
{

}

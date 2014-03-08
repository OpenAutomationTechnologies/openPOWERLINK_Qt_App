#ifndef _PROCESSIMAGEVARIABLES_H_
#define _PROCESSIMAGEVARIABLES_H_

#include "ui_ProcessImageVariables.h"
#include "Channel.h"

class ProcessImageVariables : public QFrame
{
	Q_OBJECT

public:
	explicit ProcessImageVariables(QWidget *parent = 0);

private slots:
	void on_inputCheckAll_stateChanged(int arg1);

	void on_outputCheck_stateChanged(int arg1);

	void on_inputForceAll_stateChanged(int arg1);

	void on_inputHideCheckedBtn_clicked();

	void on_inputShowAllBtn_clicked();

	void on_outHideCheckedBtn_clicked();

	void on_outShowAllBtn_clicked();

private:
	Ui::ProcessImageVariables ui;
	Channel **inputVarList;
	Channel **outputVarList;
};

#endif // _PROCESSIMAGEVARIABLES_H_

#ifndef _PROCESSIMAGEVARIABLES_H_
#define _PROCESSIMAGEVARIABLES_H_

#include "ui_ProcessImageVariables.h"
#include "ChannelUi.h"
#include <QList>

class ProcessImageVariables : public QFrame
{
	Q_OBJECT

public:
	explicit ProcessImageVariables(QWidget *parent = 0);

	Qt::CheckState GetInputSelectAllCheckBoxState() const;

	Qt::CheckState GetOutputSelectAllCheckBoxState() const;

	void UpdateInputs();

	void UpdateOutputs();

	~ProcessImageVariables();


private slots:
	void on_inputCheckAll_stateChanged(int arg1);

	void on_outputCheckAll_stateChanged(int arg1);

	void on_inputForceAll_stateChanged(int arg1);

	void on_inputHideCheckedBtn_clicked();

	void on_inputShowAllBtn_clicked();

	void on_outHideCheckedBtn_clicked();

	void on_outShowAllBtn_clicked();

private:
	Ui::ProcessImageVariables ui;
	QList<ChannelUi*> inputChannels;
	QList<ChannelUi*> outputChannels;

	void PrepareInputRows();
	void PrepareOutputRows();
};

#endif // _PROCESSIMAGEVARIABLES_H_

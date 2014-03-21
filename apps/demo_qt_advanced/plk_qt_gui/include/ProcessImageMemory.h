#ifndef _PROCESSIMAGEMEMORY_H_
#define _PROCESSIMAGEMEMORY_H_

#include "ui_ProcessImageMemory.h"

#include "user/processimage/ProcessImageIn.h"
#include "user/processimage/ProcessImageOut.h"

class ProcessImageMemory : public QFrame
{
	Q_OBJECT

public:
	explicit ProcessImageMemory(ProcessImageIn &in, ProcessImageOut &out, QWidget *parent = 0);
	~ProcessImageMemory();
public slots:
	void UpdateInputValue();
	void UpdateOutputValue();
private:
	Ui::ProcessImageMemory ui;

	ProcessImageIn &inPi;
	ProcessImageOut &outPi;

	void CreateVerticalHeaders();
	void CreateCells();
	void ResizeColumnsToContents();

	// Q_DISABLE_COPY(ProcessImageMemory)
};

#endif // _PROCESSIMAGEMEMORY_H_

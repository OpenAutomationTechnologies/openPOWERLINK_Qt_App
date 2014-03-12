#ifndef _PROCESSIMAGEMEMORY_H_
#define _PROCESSIMAGEMEMORY_H_

#include "ui_ProcessImageMemory.h"

class ProcessImageMemory : public QFrame
{
	Q_OBJECT

public:
	explicit ProcessImageMemory(QWidget *parent = 0);

private:
	Ui::ProcessImageMemory ui;
	void CreateVerticalHeaders();
	void ResizeColumnsToContents();
	// Q_DISABLE_COPY(ProcessImageMemory)
};

#endif // _PROCESSIMAGEMEMORY_H_

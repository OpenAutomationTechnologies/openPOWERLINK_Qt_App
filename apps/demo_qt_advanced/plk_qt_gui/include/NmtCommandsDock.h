#ifndef _NMT_COMMANDS_DOCK_H_
#define _NMT_COMMANDS_DOCK_H_

#include "ui_NmtCommandsDock.h"

class NmtCommandsDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit NmtCommandsDock(QWidget *parent = 0);

private slots:
	void on_sendNmtBtn_clicked();

	void on_nmtCommand_currentIndexChanged(int index);

private:
	Ui::NmtCommandsDock ui;
	// enum nmtCommand;
	// Q_DISABLE_COPY(NmtCommandsDock)
};

#endif // _NMT_COMMANDS_DOCK_H_

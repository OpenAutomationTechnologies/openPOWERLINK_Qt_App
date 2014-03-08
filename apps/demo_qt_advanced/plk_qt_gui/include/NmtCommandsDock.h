#ifndef NMTCOMMANDSDOCK_H
#define NMTCOMMANDSDOCK_H

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
};

#endif // NMTCOMMANDSDOCK_H

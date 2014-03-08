#ifndef _LOGERWINDOW_H_
#define _LOGERWINDOW_H_

#include "ui_LogerWindow.h"

class LogerWindow : public QDockWidget
{
	Q_OBJECT

public:
	explicit LogerWindow(QWidget *parent = 0);

private:
	Ui::LogerWindow ui;
};

#endif // _LOGERWINDOW_H_

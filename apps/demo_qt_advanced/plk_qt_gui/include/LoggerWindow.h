#ifndef _LOGGERWINDOW_H_
#define _LOGGERWINDOW_H_

#include "ui_LoggerWindow.h"

class LoggerWindow : public QDockWidget
{
	Q_OBJECT

public:
	explicit LoggerWindow(QWidget *parent = 0);

private:
	Ui::LoggerWindow ui;
	// Q_DISABLE_COPY(LogerWindow)
};

#endif // _LOGGERWINDOW_H_

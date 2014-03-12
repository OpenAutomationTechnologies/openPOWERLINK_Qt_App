#ifndef _ABOUTDIALOG_H_
#define _ABOUTDIALOG_H_

#include "ui_AboutDialog.h"

class AboutDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AboutDialog(QWidget *parent = 0);

private slots:
	void on_okButton_clicked();

private:
	Ui::AboutDialog ui;
};

#endif // _ABOUTDIALOG_H_

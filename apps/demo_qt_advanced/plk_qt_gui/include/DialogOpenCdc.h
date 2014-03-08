#ifndef _DIALOGOPENCDC_H_
#define _DIALOGOPENCDC_H_

#include "ui_DialogOpenCdc.h"

class DialogOpenCdc : public QDialog
{
	Q_OBJECT

public:
	explicit DialogOpenCdc(QWidget *parent = 0);

private slots:
	void on_browseCDC_clicked();

	void on_openCdcDialog_accepted();

	void on_openCdcDialog_rejected();

	void on_cdcPath_textEdited(const QString &arg1);

private:
	Ui::DialogOpenCdc ui;
	QString cdcPath;
};

#endif // _DIALOGOPENCDC_H_

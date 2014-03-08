#ifndef _SDOTRANSFER_H_
#define _SDOTRANSFER_H_

#include "ui_SdoTransfer.h"

class SdoTransfer : public QFrame
{
	Q_OBJECT

public:
	explicit SdoTransfer(QWidget *parent = 0);

private slots:
	void on_read_toggled(bool checked);

	void on_executeTransfer_clicked();

	void on_nodeId_activated(int index);

private:
	Ui::SdoTransfer ui;
};

#endif // _SDOTRANSFER_H_

#ifndef _SELECTNWINTERFACEDIALOG_H_
#define _SELECTNWINTERFACEDIALOG_H_

#include "ui_SelectNwInterfaceDialog.h"

class SelectNwInterfaceDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SelectNwInterfaceDialog(QWidget *parent = 0);
	QString GetDevName(void) const;
	QString GetDevDescription(void) const;
	int FillList(void);

private slots:
	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

	void on_buttonBox_accepted();

	void on_buttonBox_rejected();

private:
	Ui::SelectNwInterfaceDialog ui;
	QString m_devName;
	QString devDescription;
};

#endif // _SELECTNWINTERFACEDIALOG_H_

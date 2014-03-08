#ifndef SELECTNWINTERFACEDIALOG_H
#define SELECTNWINTERFACEDIALOG_H

#include "ui_SelectNwInterfaceDialog.h"

class SelectNwInterfaceDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SelectNwInterfaceDialog(QWidget *parent = 0);
	QString getDevName(void);
	int fillList(void);

private slots:
	void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
	Ui::SelectNwInterfaceDialog ui;
	QString m_devName;
};

#endif // SELECTNWINTERFACEDIALOG_H

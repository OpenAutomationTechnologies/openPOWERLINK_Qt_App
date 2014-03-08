#include "DialogOpenCdc.h"
#include <QFileDialog>
#include <QFile>

DialogOpenCdc::DialogOpenCdc(QWidget *parent) :
	QDialog(parent)
{
	this->ui.setupUi(this);
}

void DialogOpenCdc::on_browseCDC_clicked()
{
	// for 3rd parameter use default path or last used path using QSettings
	this->cdcPath = QFileDialog::getOpenFileName(this, "Choose CDC...",
					 QString(), tr("Concise Device Configuration File(*.cdc *.CDC)"));
	qDebug(this->cdcPath.toStdString().c_str());
	this->ui.cdcPath->setText(this->cdcPath);
}

void DialogOpenCdc::on_openCdcDialog_accepted()
{
	// CDC file will be exist because it is selected via ui. If not the user pasted the value
	// Also check for is readable and symlink QFileInfo
	if(QFile::exists(this->cdcPath))
	{

	}
	this->on_browseCDC_clicked();
	// Strip /*.cdc and append xap.xml and check if file exists. and return error here.
}

void DialogOpenCdc::on_openCdcDialog_rejected()
{

}

void DialogOpenCdc::on_cdcPath_textEdited(const QString &arg1)
{
	this->on_browseCDC_clicked();
}

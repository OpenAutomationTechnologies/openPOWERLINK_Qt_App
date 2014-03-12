#include "AboutDialog.h"
#include <QDateTime>

const QString buildTime = QDateTime::currentDateTimeUtc().toString(Qt::RFC2822Date);

//#if defined(_WIN32)
//	#if defined(_MSC_VER)
//	const uint buildVersion = _MSC_FULL_VER; // _MSC_VER
//	const QString compilerName = "MSVC";
//	#endif
//#elif defined(_linux_)
//	#if defined(__GNUG__)
//	const uint buildVersion = _MSC_FULL_VER; // _MSC_VER
//	#define GCC_VERSION (__GNUC__ * 10000 \
//							   + __GNUC_MINOR__ * 100 \
//							   + __GNUC_PATCHLEVEL__)
//	const QString compilerName = "gcc/g++";
//	#endif
//#endif

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent)
{
	ui.setupUi(this);
	this->ui.builtOn->setText(buildTime);
	this->ui.stackVersion->setText("oplk v2.0-b1");
	// qDebug(qPrintable(QString::number(buildVersion)));
}

void AboutDialog::on_okButton_clicked()
{
	this->accept();
}

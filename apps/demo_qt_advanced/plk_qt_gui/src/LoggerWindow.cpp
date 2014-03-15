#include "LoggerWindow.h"

#include "api/OplkQtApi.h"

LoggerWindow::LoggerWindow(QWidget *parent) :
	QDockWidget(parent)
{
	this->ui.setupUi(this);
	OplkQtApi::RegisterEventLogger(*this, this->metaObject()->method(
			this->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(
				"HandlePrintLog(const QString&)").constData())));

}

void LoggerWindow::HandlePrintLog(const QString& str)
{
	this->ui.txtBxConsoleLog->appendPlainText(str);
}

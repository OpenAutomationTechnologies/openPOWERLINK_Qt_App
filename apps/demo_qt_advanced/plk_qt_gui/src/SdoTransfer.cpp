/**
********************************************************************************
\file   SdoTransfer.cpp

\brief  Implements the SDO Transfer user interface actions and results.

\todo
		- Implement V-String and QString
		- Validate signed values.

\author Ramakrishnan Periyakaruppan

\copyright (c) 2014, Kalycito Infotech Private Limited
					 All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holders nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <QMessageBox>
#include <QDateTime>

#include "SdoTransfer.h"
#include "api/OplkQtApi.h"
#include "oplk/debugstr.h"

/*******************************************************************************
* Static functions
*******************************************************************************/
QMap<QString, QMetaType::Type> SdoTransfer::CreateDataTypeMap()
{
	QMap<QString, QMetaType::Type> dataType;
	// Using q*** datatype to maintain compatibility between platforms
	dataType["SIGNED8"]        = (QMetaType::Type)QMetaType::type("qint8");
	dataType["UNSIGNED8"]      = (QMetaType::Type)QMetaType::type("quint8");
	dataType["SIGNED16"]       = (QMetaType::Type)QMetaType::type("qint16");
	dataType["UNSIGNED16"]     = (QMetaType::Type)QMetaType::type("quint16");
	dataType["SIGNED32"]       = (QMetaType::Type)QMetaType::type("qint32");
	dataType["UNSIGNED32"]     = (QMetaType::Type)QMetaType::type("quint32");
	dataType["SIGNED64"]       = (QMetaType::Type)QMetaType::type("qint64");
	dataType["UNSIGNED64"]     = (QMetaType::Type)QMetaType::type("quint64");
	/*
	 * TODO Implement
	dataType["VISIBLE STRING"] = (QMetaType::Type)QMetaType::type("QString");
	dataType["OCTET STRING"]   = (QMetaType::Type)QMetaType::type("QString");
	*/
	return dataType;
}

/*******************************************************************************
* Static Member Variables
*******************************************************************************/
const QMap<QString, QMetaType::Type> SdoTransfer::dataTypeMap = SdoTransfer::CreateDataTypeMap();

//TODO can have a qmap for sdoVia also

/*******************************************************************************
* Private functions
*******************************************************************************/
SdoTransfer::SdoTransfer(QWidget *parent) :
	QFrame(parent),
	sdoViaUdpStr("UDP"),
	sdoViaASndStr("ASnd"),
	sdoTransferJob(NULL),
	sdoTransferData(0),
	maxDataValue(0),
	minDataValue(0),
	metaDataTypeIndex(QMetaType::UnknownType),
	sdoValueValidator(NULL)
{
	this->ui.setupUi(this);

	this->ui.sdoVia->addItem(this->sdoViaASndStr);
#ifndef _WIN32
	this->ui.sdoVia->addItem(this->sdoViaUdpStr);
#endif

	const QString receiverFunctionstr = "HandleSdoTransferFinished(const SdoTransferResult)";
	const INT sdoResultHandlerIndex = this->metaObject()->indexOfMethod(
									QMetaObject::normalizedSignature(
									receiverFunctionstr.toUtf8().constData()));
	Q_ASSERT(sdoResultHandlerIndex != -1);
	// If asserted check for the receiverFunction name
	this->receiverFunction = this->metaObject()->method(sdoResultHandlerIndex);
}

void SdoTransfer::on_read_toggled(bool selected)
{
	this->ui.sdoResultValue->clear();
	this->ui.sdoResultValueHex->clear();

	if (!selected)
		this->ui.sdoResultValueHex->hide();
	else
		this->ui.sdoResultValueHex->show();

	this->ui.sdoResultValue->setDisabled(selected);
}

void SdoTransfer::on_executeTransfer_clicked()
{

	/* Get the user inputs from the UI */
	bool conversionSuccess = false;
	// Using C language conversion
	const UINT nodeId = this->ui.nodeId->currentText().toUInt(&conversionSuccess, 0);
	if (!conversionSuccess)
	{
		QMessageBox::warning(this, "SDO Transfer - Failed",
							 "Invalid node id",
							 QMessageBox::Close);
		return;
	}

	const UINT index = this->ui.index->value(); // -ve values are not emitted
	const UINT subIndex = this->ui.subIndex->value(); // -ve values are not emitted
	// QString dataTypeStr = this->ui.dataType->currentText();

	//SDO transfer protocol from selection
	tSdoType sdoProtocol = kSdoTypeAuto;
	if (this->ui.sdoVia->currentText() == this->sdoViaASndStr)
	{
		sdoProtocol = kSdoTypeAsnd;
	}
	else if (this->ui.sdoVia->currentText() == this->sdoViaUdpStr)
	{
		sdoProtocol = kSdoTypeUdp;
	}

	//Choose the SDO transfer type from selection
	tSdoAccessType sdoAccessType = kSdoAccessTypeRead;
	if (this->ui.read->isChecked())
	{
		sdoAccessType = kSdoAccessTypeRead;
		this->ui.sdoResultValue->clear();
		this->ui.sdoResultValueHex->clear();

		this->sdoTransferData = QVariant(this->metaDataTypeIndex, NULL);
	}
	else if (this->ui.write->isChecked())
	{
		sdoAccessType = kSdoAccessTypeWrite;
		if (this->ui.sdoResultValue->text().isEmpty())
		{
			QMessageBox::warning(this, "SDO Transfer - Failed",
								 "Enter a valid value",
								 QMessageBox::Close);
			return;
		}
	}

	/* Update the transfer status */
	this->ui.transferStatus->setText("Transferring...");

	/* disable the user input objects */
	this->ui.groupBoxSdoTransfer->setEnabled(false);
	// Prepare the SDO transfer job
	this->sdoTransferJob = new SdoTransferJob(nodeId,
									index,
									subIndex,
									(void*)(&(this->sdoTransferData)),
									QMetaType::sizeOf(this->metaDataTypeIndex),
									sdoProtocol,
									sdoAccessType);

	qDebug("Max: %d Min:%d", this->maxDataValue, this->minDataValue);
	tOplkError oplkRet =  OplkQtApi::TransferObject(*(this->sdoTransferJob),
										*(this),
										this->receiverFunction);

	//update the SDO log
	QString logMessage = QString("SDO transfer %1 (Node=%2, Data type=%3, Index=0x%4, Sub index=0x%5 via=%6")
						 .arg(((sdoAccessType == kSdoAccessTypeRead) ? "Read" : "Write"))
						 .arg(QString::number(nodeId, 10))
						 .arg(this->ui.dataType->currentText())
						 .arg(QString::number(index, 16))
						 .arg(QString::number(subIndex, 16))
						 .arg(this->ui.sdoVia->currentText());

	if (sdoAccessType == kSdoAccessTypeWrite)
	{
//		logMessage.append(QString(" Value=%1(0x%2)")
//				.arg(QString::number(this->ui.sdoResultValue->text().toULongLong(), 10))
//				.arg(this->ui.sdoResultValue->text(), 0, 16));
		logMessage.append(QString(" Value=%1")
				.arg(this->ui.sdoResultValue->text()));
	}
	else
	{
		logMessage.append(")");
	}

	this->UpdateLog(logMessage);

	//handle the return from the api
	if (oplkRet == kErrorOk)
	{
		//Local OD access successful
		this->ui.transferStatus->setText("Transfer completed");
		this->UpdateLog(QString("SdoTransfer completed successfully"));

		if (this->ui.read->isChecked())
			this->UpdateSdoTransferReturnValue();

		this->ui.groupBoxSdoTransfer->setEnabled(true);
	}
	else if (oplkRet == kErrorApiTaskDeferred)
	{
		//Remote OD access, shall be handled in the callback
		this->ui.transferStatus->setText("Transfer Initiated");
	}
	else
	{
		//Failed OD Access
		QString errorMessage = QString("SDO transfer failed. Err=0x%1(%2)")
									.arg(QString::number(oplkRet, 16))
									.arg(debugstr_getRetValStr(oplkRet));
		this->ui.transferStatus->setText(errorMessage);
		this->UpdateLog(errorMessage);
		this->ui.groupBoxSdoTransfer->setEnabled(true);
	}

	if (this->sdoTransferJob != NULL)
	{
		delete this->sdoTransferJob;
	}
}

void SdoTransfer::HandleSdoTransferFinished(const SdoTransferResult result)
{
	this->ui.transferStatus->setText("");
	//update the SDO log
	this->UpdateLog(QString("Transfer finished (Node=%1, Index=0x%2, Sub index=0x%3 Result=0x%4(%5))")
			.arg(QString::number(result.GetNodeId(), 10))
			.arg(QString::number(result.GetIndex(), 16))
			.arg(QString::number(result.GetSubIndex(), 16))
			.arg(QString::number(result.GetSdoComConState(), 16))
			.arg(debugstr_getSdoComConStateStr(result.GetSdoComConState())));

	// Check the result of Remote Node OD access from the abort codes
	if ((result.GetAbortCode() != 0)
		|| (result.GetSdoComConState() != kSdoComTransferFinished))
	{
		QString abortmsg = QString("Abort code=0x%1(%2)")
								.arg(QString::number(result.GetAbortCode(), 16))
								.arg(QString::fromStdString(
									SdoTransferResult::GetAbortCodeDescription(
											 result.GetAbortCode())));
		this->ui.transferStatus->setText(abortmsg);
		this->UpdateLog(abortmsg);
	}
	else
	{
		this->ui.transferStatus->setText("Transfer Completed");
		qDebug("Writedata %u", (*((quint64*)(&(this->sdoTransferData)))));

		if (this->ui.read->isChecked())
			this->UpdateSdoTransferReturnValue();
	}

	this->ui.groupBoxSdoTransfer->setEnabled(true);
}

void SdoTransfer::on_dataType_currentIndexChanged(const QString &dataType)
{
	this->ui.sdoResultValue->clear();
	this->ui.sdoResultValueHex->clear();
	this->metaDataTypeIndex = SdoTransfer::dataTypeMap[dataType];
	this->SetMaskForValue();
	this->ui.sdoResultValue->setValidator(this->sdoValueValidator);
}

void SdoTransfer::SetMaskForValue()
{
	const UINT sdoDataSize = QMetaType::sizeOf(this->metaDataTypeIndex);

	this->sdoTransferData = QVariant(this->metaDataTypeIndex, NULL);

	// SEtting to FFFFF...
	quint64 maxUnsignedDataVal = (~((quint64)0x0));
	maxUnsignedDataVal = (~((quint64)(maxUnsignedDataVal << (sdoDataSize * 8))));

	switch (this->metaDataTypeIndex)
	{
		case QMetaType::UInt:
		case QMetaType::ULong:
		case QMetaType::ULongLong:
		case QMetaType::UShort:
		case QMetaType::UChar:
		{
			this->maxDataValue = maxUnsignedDataVal;
			this->minDataValue = 0;

			UINT decDataLen = 0;

			do
			{
				decDataLen++;
			} while ((maxUnsignedDataVal /= 10) > 0);

			if (this->sdoValueValidator != NULL)
			{
				delete this->sdoValueValidator;
			}

			QRegExp validateStr(QString("^(0x[0-9A-Fa-f]{1,%1})|([0-9]{1,%2})$")
								.arg(sdoDataSize * 2)
								.arg(decDataLen));
			this->sdoValueValidator = new QRegExpValidator(validateStr, this);
			break;
		}
		case QMetaType::Int:
		case QMetaType::Long:
		case QMetaType::LongLong:
		case QMetaType::Short:
		case QMetaType::SChar:
		{
			maxUnsignedDataVal = (quint64)(maxUnsignedDataVal / 2);
			this->maxDataValue = maxUnsignedDataVal;
			this->minDataValue = 0 -(maxUnsignedDataVal + 1);

			UINT decDataLen = 0;

			do
			{
				++decDataLen;
			} while ((maxUnsignedDataVal /= 10) > 0);

			QRegExp validateStr(QString("^(0x[0-9A-Fa-f]{1,%1})|([-]?[0-9]{1,%2})$")
								.arg(sdoDataSize * 2)
								.arg(decDataLen));

			if (this->sdoValueValidator != NULL)
			{
				delete this->sdoValueValidator;
			}

			this->sdoValueValidator = new QRegExpValidator(validateStr, this);
			break;
		}
		case QMetaType::QString:
		case QMetaType::QChar:
		{
			//Only V-String is handled
			QRegExp validateStr(QString("^[!\"#$%&'()*+,-./0-9:;<=>?\A-Z[\\]^_`a-z{|}~]{0,%1}$")
								.arg(sdoDataSize));

			if (this->sdoValueValidator != NULL)
			{
				delete this->sdoValueValidator;
			}

			this->sdoValueValidator = new QRegExpValidator(validateStr, this);

			this->sdoTransferData = 0;
			break;
		}
		default:
			qDebug("Unhandled datatype from %s %d", __FUNCTION__, this->metaDataTypeIndex);
			break;
	}
}

void SdoTransfer::UpdateSdoTransferReturnValue()
{
	const UINT sdoDataSize = QMetaType::sizeOf(this->metaDataTypeIndex);

	qDebug("sdoDataSize %d", sdoDataSize);

	switch (this->metaDataTypeIndex)
	{
		case QMetaType::UInt:
		case QMetaType::ULong:
		case QMetaType::ULongLong:
		case QMetaType::UShort:
		case QMetaType::UChar:
		{
			bool res;
			quint64 data = this->sdoTransferData.toULongLong(&res);
			if (!res)
			{
				qDebug("Conversion failed ULONGLONG. MetaType %d", this->metaDataTypeIndex);
			}

			this->ui.sdoResultValue->setText(QString("%1").arg(data));
			this->ui.sdoResultValueHex->setText(QString("0x%1")
						.arg(data, (sdoDataSize * 2), 16, QLatin1Char('0')));
			break;
		}
		case QMetaType::Int:
		case QMetaType::Long:
		case QMetaType::LongLong:
		case QMetaType::Short:
		case QMetaType::SChar:
		{
			bool res;
			qint64 data = this->sdoTransferData.toLongLong(&res);
			if (!res)
			{
				qDebug("Conversion failed LONGLONG. MetaType %d", this->metaDataTypeIndex);
			}

			this->ui.sdoResultValue->setText(QString("%1").arg(data));
			this->ui.sdoResultValueHex->setText(QString("0x%1")
						.arg(data, (sdoDataSize * 2), 16, QLatin1Char('0')));
			break;
		}
		case QMetaType::QString:
		case QMetaType::QChar:
		{
			bool res;
			quint64 data = this->sdoTransferData.toULongLong(&res);
			if (!res)
			{
				qDebug("Conversion failed ULONGLONG. MetaType %d", this->metaDataTypeIndex);
			}

			char hexArray [8] = {0};

			for (UINT8 i = 0; i < 8; ++i)
			{
				hexArray[i] = (UINT8)(data >> (8 * i));
			}
			QByteArray byteArray = QByteArray::fromRawData(hexArray, 8);
			this->ui.sdoResultValue->setText(QString("%1").arg(byteArray.constData()));
			break;
		}
		default:
			qDebug("Unhandled datatype from %s %d", __FUNCTION__, this->metaDataTypeIndex);
			break;
	}
}

bool SdoTransfer::IsValidValue()
{
	bool result = false;
	switch (this->metaDataTypeIndex)
	{
		case QMetaType::UInt:
		case QMetaType::ULong:
		case QMetaType::ULongLong:
		case QMetaType::UShort:
		case QMetaType::UChar:
		{
			bool res;
			quint64 data = this->ui.sdoResultValue->text().toULongLong(&res, 0);
			if (!res)
			{
				qDebug("Conversion failed ULONGLONG. MetaType %d", this->metaDataTypeIndex);
			}

			//check the data size
			if ((data <= this->maxDataValue) || (!res))
			{
				result = true;
			}
			else
			{
				data = 0;
			}

			this->sdoTransferData = QVariant(this->metaDataTypeIndex, (void*)&data);
			break;
		}
		case QMetaType::Int:
		case QMetaType::Long:
		case QMetaType::LongLong:
		case QMetaType::Short:
		case QMetaType::SChar:
		{
			bool res;
			qint64 data = this->ui.sdoResultValue->text().toLongLong(&res, 0);
			if (!res)
			{
				qDebug("Conversion failed LONGLONG. MetaType %d", this->metaDataTypeIndex);
			}

			//TODO if loop double check. failing
			//check the data size
			if (((this->maxDataValue - data) >= 0)
					|| (data >= this->minDataValue)
					|| (!res))
			{
				result = true;
			}
			else
			{
				data = 0;
			}

			this->sdoTransferData = QVariant(this->metaDataTypeIndex, (void*)&data);
			break;
		}
		case QMetaType::QString:
		case QMetaType::QChar:
		{
			// TODO Implement
			// QString sdoValueStr = this->ui.sdoResultValue->text();
			//TODO: Doesnt take qstring some workaround neeeded
			// this->sdoTransferData = QVariant(dataTypeMap["QString"], (void*)&(sdoValueStr.toStdString().c_str()));
			break;
		}
		default:
			qDebug("Unhandled datatype from %s %d", __FUNCTION__, this->metaDataTypeIndex);
			break;
	}
	return result;
}

void SdoTransfer::on_sdoResultValue_editingFinished()
{
	qDebug("editing finishd");
	if (!(this->IsValidValue()))
	{
		qDebug("InvalidValue");
		this->ui.transferStatus->setText("Datatype-Value Mismatch");
		this->ui.sdoResultValue->clear();
	}
}

//TODO change to AddToNodeList
void SdoTransfer::UpdateNodeList(unsigned int nodeId)
{
	this->ui.nodeId->hidePopup();
	//TODO move to constructor or ui
	this->ui.nodeId->setDuplicatesEnabled(false);
	this->ui.nodeId->setInsertPolicy(QComboBox::InsertAlphabetically);

	QStringList list;
	for (UINT it = 0; it < this->ui.nodeId->count(); ++it)
	{
		list << this->ui.nodeId->itemText(it);
	}
	list << QString("%1").arg(nodeId, 3, 10, QChar('0'));

	list.sort();

	this->ui.nodeId->clear();
	this->ui.nodeId->addItems(list);
}

void SdoTransfer::RemoveFromNodeList(unsigned int nodeId)
{
	this->ui.nodeId->hidePopup();
	QString nodeIdToRemove = QString("%1").arg(nodeId, 3, 10, QChar('0'));
	QStringList list;
	for (UINT it = 0; it < this->ui.nodeId->count(); ++it)
	{
		if (this->ui.nodeId->itemText(it).compare(nodeIdToRemove) != 0)
			list << this->ui.nodeId->itemText(it);
	}

	list.sort();

	this->ui.nodeId->clear();
	this->ui.nodeId->addItems(list);
}

void SdoTransfer::UpdateLog(const QString& logMessage)
{
	QString log;

	log.append(QDateTime::currentDateTime().toString("yyyy/MM/dd-hh:mm:ss.zzz"));
	log.append(" - ");
	log.append(logMessage);

	emit SignalSdoLog(log);
}

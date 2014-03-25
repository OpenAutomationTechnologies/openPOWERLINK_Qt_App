/**
********************************************************************************
\file   SdoTransfer.cpp

\brief

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

	this->ui.dataType->clear();

	UINT index;
	for (QMap<QString, QMetaType::Type>::const_iterator it = SdoTransfer::dataTypeMap.begin();
			it != SdoTransfer::dataTypeMap.end(); ++it)
	{
		index = 0;
		this->ui.dataType->insertItem(index, it.key());
		// Setting the default dataType.
		if ((QMetaType::Type)(it.value()) == QMetaType::UInt)
		{
			this->ui.dataType->setCurrentText(it.key());
		}

		++index;
	}

	this->ui.sdoVia->addItem(this->sdoViaASndStr);
#ifndef _WIN32
	this->ui.sdoVia->addItem(this->sdoViaUdpStr);
#endif

	QString receiverFunction = "HandleSdoTransferFinished(const SdoTransferResult)";
	const INT sdoResultHandlerIndex = this->metaObject()->indexOfMethod(
									QMetaObject::normalizedSignature(receiverFunction.toUtf8().constData()));
	Q_ASSERT(sdoResultHandlerIndex != -1);
	// If asserted check for the receiverFunction name
	this->receiverMetaObject = this->metaObject()->method(sdoResultHandlerIndex);
}

void SdoTransfer::on_read_toggled(bool checked)
{
	this->ui.sdoResultValue->clear();
	this->ui.sdoResultValue->setReadOnly(checked);
}

void SdoTransfer::on_executeTransfer_clicked()
{
/* disable the user input objects */
	this->ui.groupBoxSdoTransfer->setEnabled(false);

/* Update the transfer status */
	this->ui.transferStatus->setText("Transferring...");

/* Get the user inputs from the UI */
	bool conversionSuccess = false;
	// Using C language conversion
	const UINT nodeId = this->ui.nodeId->currentText().toUInt(&conversionSuccess, 0);
	if (!conversionSuccess)
	{
		qDebug("Invalid Nodeid");
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
	}
	else if (this->ui.write->isChecked())
	{
		sdoAccessType = kSdoAccessTypeWrite;
	}

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
										this->receiverMetaObject);

	//update the SDO log
	this->ui.sdoTransferLog->append(QString("\n\n%1 Initialized for NodeId: 0x%2, DataType:%3, Index:%4, SubIndex:%5 via: %6")
											.arg(((sdoAccessType == kSdoAccessTypeRead) ? "Read" : "Write"))
											.arg(QString::number(nodeId, 16))
											.arg(this->ui.dataType->currentText())
											.arg(QString::number(index, 16))
											.arg(QString::number(subIndex, 16))
											.arg(this->ui.sdoVia->currentText()));
	if (sdoAccessType == kSdoAccessTypeWrite)
	{
		this->ui.sdoTransferLog->append(
				QString("WriteValue: %1")
				.arg(this->ui.sdoResultValue->text(), 0, 16));
	}

	//handle the return from the api
	if (oplkRet == kErrorOk)
	{
		//Local OD access successful
		this->ui.transferStatus->setText("Last Transfer Completed");
		this->ui.sdoTransferLog->append(QString("SdoTransfer finished successfully"));
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
		QString errorMessage = QString("Transfer Aborted.\n%1")
									.arg(debugstr_getRetValStr(oplkRet));
		this->ui.transferStatus->setText(errorMessage);
		this->ui.sdoTransferLog->append(errorMessage);
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
	this->ui.sdoTransferLog->append(QString("TransferFinished for NodeId:0x%1, Index:0x%2, SubIndex:0x%3 Transfer Result:%4")
			.arg(QString::number(result.GetNodeId(), 16))
			.arg(QString::number(result.GetIndex(), 16))
			.arg(QString::number(result.GetSubIndex(), 16))
			.arg(debugstr_getSdoComConStateStr(result.GetSdoComConState())));

	// Check the result of Remote Node OD access from the abort codes
	if (result.GetAbortCode() != 0)
	{
		this->ui.transferStatus->setText(QString("Transfer Aborted.\n0x%1(%2)")
									.arg(QString::number(result.GetAbortCode(), 16))
									.arg(this->GetAbortCodeString(result.GetAbortCode())));
	}
	else
	{
		this->ui.transferStatus->setText("Last Transfer Completed");
		qDebug("RAM %u", (*((quint64*)(&(this->sdoTransferData)))));
		this->UpdateSdoTransferReturnValue();
	}

	this->ui.groupBoxSdoTransfer->setEnabled(true);
}

void SdoTransfer::on_dataType_currentIndexChanged(const QString &dataTypeStr)
{
	this->ui.sdoResultValue->clear();
	this->metaDataTypeIndex = SdoTransfer::dataTypeMap[dataTypeStr];
	this->SetMaskForValue();
	this->ui.sdoResultValue->setValidator(this->sdoValueValidator);
}

void SdoTransfer::SetMaskForValue()
{
	const UINT sdoDataSize = QMetaType::sizeOf(this->metaDataTypeIndex);

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

			this->sdoTransferData = QVariant(this->metaDataTypeIndex, NULL);
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

			this->sdoTransferData = QVariant(this->metaDataTypeIndex, NULL);
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

			this->ui.sdoResultValue->setText(QString("0x%1")
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

			this->ui.sdoResultValue->setText(QString("0x%1")
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
			if ((data <= this->maxDataValue) || (data >= 0) || (!res))
			{
				data = 0;
				result = true;
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

			//check the data size
			if (((this->maxDataValue - data) >= 0)
					|| (data >= this->minDataValue) || (!res))
			{
				data = 0;
				result = true;
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

void SdoTransfer::GetConfiguredNodeIdList(QStringList &nodeIdList)
{
	DWORD nodeAssignment;
	UINT size = sizeof (nodeAssignment);
	// Can read upto subindex 00th
	for (UINT subIndex = 1; subIndex < 240; ++subIndex)
	{
//		SdoTransferJob transferjob = SdoTransferJob(0xF0,
//									0x1F81,
//									subIndex,
//									(void*)(&nodeAssignment),
//									QMetaType::sizeOf(this->metaDataTypeIndex),
//									kSdoTypeAsnd,
//									kSdoAccessTypeRead);
//		tOplkError oplkRet =  OplkQtApi::TransferObject(transferjob,
//										*(this),
//										this->receiverMetaObject);

		/* TODO Implement new API in library for easy use.
		 *  There is no need of preparing the receiver and reciever function here
		 * */
		tOplkError oplkRet = oplk_readLocalObject(0x1F81, subIndex,
										(void*)(&nodeAssignment),
										&size);
		if (oplkRet != kErrorOk)
		{
			qDebug("Local Read Error: %s", debugstr_getRetValStr(oplkRet));
		}

		if (nodeAssignment != 0)
		nodeIdList.push_back(QString("0x%1").arg(subIndex, 2, 16, QLatin1Char('0')));
	}
	nodeIdList.push_back("0xF0");
}

const QString SdoTransfer::GetAbortCodeString(const UINT32 abortCode) const
{
	/* TODO Move this to API of SDO transfer
	 * */
	QString abortStr;
	switch (abortCode)
	{
		case SDO_AC_TIME_OUT:
		{
			abortStr = "SDO protocol timed out";
			break;
		}
		case SDO_AC_UNKNOWN_COMMAND_SPECIFIER:
		{
			abortStr = "Client/server Command ID not valid or unknown";
			break;
		}
		case SDO_AC_INVALID_BLOCK_SIZE:
		{
			abortStr = "Invalid block size";
			break;
		}
		case SDO_AC_INVALID_SEQUENCE_NUMBER:
		{
			abortStr = "Invalid sequence number";
			break;
		}
		case SDO_AC_OUT_OF_MEMORY:
		{
			abortStr = "Out of memory";
			break;
		}
		case SDO_AC_UNSUPPORTED_ACCESS:
		{
			abortStr = "Unsupported access to an object";
			break;
		}
		case SDO_AC_READ_TO_WRITE_ONLY_OBJ:
		{
			abortStr = "Attempt to read a write-only object";
			break;
		}
		case SDO_AC_WRITE_TO_READ_ONLY_OBJ:
		{
			abortStr = "Attempt to write a read-only object";
			break;
		}
		case SDO_AC_OBJECT_NOT_EXIST:
		{
			abortStr = "Object does not exist in the object dictionary";
			break;
		}
		case SDO_AC_OBJECT_NOT_MAPPABLE:
		{
			abortStr = "Object cannot be mapped to the PDO";
			break;
		}
		case SDO_AC_PDO_LENGTH_EXCEEDED:
		{
			abortStr = "The number and length of the objects to be mapped would exceed PDO length";
			break;
		}
		case SDO_AC_GEN_PARAM_INCOMPATIBILITY:
		{
			abortStr = "General parameter incompatibility";
			break;
		}
		case SDO_AC_INVALID_HEARTBEAT_DEC:
		{
			abortStr = "Invalid heartbeat declaration";
			break;
		}
		case SDO_AC_GEN_INTERNAL_INCOMPATIBILITY:
		{
			abortStr = "General internal incompatibility in the device";
			break;
		}
		case SDO_AC_ACCESS_FAILED_DUE_HW_ERROR:
		{
			abortStr = "Access failed due to an hardware error";
			break;
		}
		case SDO_AC_DATA_TYPE_LENGTH_NOT_MATCH:
		{
			abortStr = "Data type does not match, length of service parameter does not match";
			break;
		}
		case SDO_AC_DATA_TYPE_LENGTH_TOO_HIGH:
		{
			abortStr = "Data type does not match, length of service parameter too high";
			break;
		}
		case SDO_AC_DATA_TYPE_LENGTH_TOO_LOW:
		{
			abortStr = "Data type does not match, length of service parameter too low";
			break;
		}
		case SDO_AC_SUB_INDEX_NOT_EXIST:
		{
			abortStr = "Sub-index does not exist";
			break;
		}
		case SDO_AC_VALUE_RANGE_EXCEEDED:
		{
			abortStr = "Value range of parameter exceeded (only for write access)";
			break;
		}
		case SDO_AC_VALUE_RANGE_TOO_HIGH:
		{
			abortStr = "Value of parameter written too high";
			break;
		}
		case SDO_AC_VALUE_RANGE_TOO_LOW:
		{
			abortStr = "Value of parameter written too low";
			break;
		}
		case SDO_AC_MAX_VALUE_LESS_MIN_VALUE:
		{
			abortStr = "Maximum value is less than minimum value";
			break;
		}
		case SDO_AC_GENERAL_ERROR:
		{
			abortStr = "General error";
			break;
		}
		case SDO_AC_DATA_NOT_TRANSF_OR_STORED:
		{
			abortStr = "Data cannot be transferred or stored to the application";
			break;
		}
		case SDO_AC_DATA_NOT_TRANSF_DUE_LOCAL_CONTROL:
		{
			abortStr = "Data cannot be transferred or stored to the application because of local control";
			break;
		}
		case SDO_AC_DATA_NOT_TRANSF_DUE_DEVICE_STATE:
		{
			abortStr = "Data cannot be transferred or stored to the application because of the present device state";
			break;
		}
		case SDO_AC_OBJECT_DICTIONARY_NOT_EXIST:
		{
			abortStr = "Object dictionary dynamic generation fails or no object dictionary is present (e.g. object dictionary is generated from file and generation fails because of a file error)";
			break;
		}
		case SDO_AC_CONFIG_DATA_EMPTY:
		{
			abortStr = "EDS, DCF or Concise DCF Data set empty";
			break;
		}
		default:
			abortStr = QString::number(abortCode, 16);
			break;
	}

	return abortStr;
}


void SdoTransfer::on_updateNodeListButton_clicked()
{
	this->ui.nodeId->clear();
	QStringList nodeIdList;
	this->GetConfiguredNodeIdList(nodeIdList);
	this->ui.nodeId->insertItems(0, nodeIdList);
}

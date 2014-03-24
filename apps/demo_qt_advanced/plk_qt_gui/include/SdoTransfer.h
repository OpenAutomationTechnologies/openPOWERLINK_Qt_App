#ifndef _SDOTRANSFER_H_
#define _SDOTRANSFER_H_

#include "ui_SdoTransfer.h"
#include "user/SdoTransferResult.h"
#include "user/SdoTransferJob.h"

#include <QMetaMethod>
#include <QMap>

class SdoTransfer : public QFrame
{
	Q_OBJECT

public:
	explicit SdoTransfer(QWidget *parent = 0);

private slots:
	void on_read_toggled(bool checked);

	void on_executeTransfer_clicked();

	void on_nodeId_activated(int index);

	void on_dataType_currentIndexChanged(const QString &arg1);

	void on_sdoResultValue_editingFinished();

private:
	Ui::SdoTransfer ui;

	QVariant sdoTransferData;

	QValidator *sdoValueValidator;
	quint64 maxDataValue;
	qint64 minDataValue;

//TODO can have a qmap for sdoVia also
	const QString sdoViaUdpStr;
	const QString sdoViaASndStr;

	SdoTransferJob *sdoTransferJob;

	// receiver object should be a part of the class object.
	// Because the receiver function needs the objects memory while for RemoteSDO Transfer.
	QMetaMethod receiverMetaObject;
	QMetaType::Type metaDataTypeIndex;

	static QMap<QString, QMetaType::Type> CreateDataTypeMap();
	static const QMap<QString, QMetaType::Type> dataTypeMap;

	Q_INVOKABLE void HandleSdoTransferFinished(const SdoTransferResult result);

	bool IsValidValue();
	void UpdateSdoTransferReturnValue();
	void SetMaskForValue();

	// TODO Has to be moved to API library or stack
	QString GetAbortCodeString(UINT32 abortCode);
	void GetConfiguredNodeIdList(QStringList &nodeIdList);

};

#endif // _SDOTRANSFER_H_

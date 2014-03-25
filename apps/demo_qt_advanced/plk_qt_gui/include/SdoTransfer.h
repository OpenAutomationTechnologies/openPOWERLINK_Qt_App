/**
********************************************************************************
\file   SdoTransfer.h

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

#ifndef _SDOTRANSFER_H_
#define _SDOTRANSFER_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "ui_SdoTransfer.h"
#include "user/SdoTransferResult.h"
#include "user/SdoTransferJob.h"

#include <QMetaMethod>
#include <QMap>

/**
 * \brief The SdoTransfer class
 */
class SdoTransfer : public QFrame
{
	Q_OBJECT

public:
	/**
	 * \brief SdoTransfer
	 * \param parent
	 */
	explicit SdoTransfer(QWidget *parent = 0);

private slots:
	/**
	 * \brief on_read_toggled
	 * \param checked
	 */
	void on_read_toggled(bool checked);

	/**
	 * \brief on_executeTransfer_clicked
	 */
	void on_executeTransfer_clicked();

	/**
	 * \brief on_dataType_currentIndexChanged
	 * \param arg1
	 */
	void on_dataType_currentIndexChanged(const QString &arg1);

	/**
	 * \brief on_sdoResultValue_editingFinished
	 */
	void on_sdoResultValue_editingFinished();

	/**
	 * \brief on_updateNodeListButton_clicked
	 */
	void on_updateNodeListButton_clicked();

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

	/**
	 * \brief CreateDataTypeMap
	 * \return
	 */
	static QMap<QString, QMetaType::Type> CreateDataTypeMap();
	static const QMap<QString, QMetaType::Type> dataTypeMap;

	/**
	 * \brief
	 *
	 * \param[in] result
	 */
	Q_INVOKABLE void HandleSdoTransferFinished(const SdoTransferResult result);

	/**
	 * \brief IsValidValue
	 * \return
	 */
	bool IsValidValue();

	/**
	 * \brief UpdateSdoTransferReturnValue
	 */
	void UpdateSdoTransferReturnValue();

	/**
	 * \brief SetMaskForValue
	 */
	void SetMaskForValue();

	// TODO Has to be moved to API library or stack
	/**
	 * \brief GetAbortCodeString
	 * \param[in] abortCode
	 * \return
	 */
	const QString GetAbortCodeString(const UINT32 abortCode) const;

	/**
	 * \brief GetConfiguredNodeIdList
	 * \param[out] nodeIdList
	 */
	void GetConfiguredNodeIdList(QStringList &nodeIdList);

};

#endif // _SDOTRANSFER_H_

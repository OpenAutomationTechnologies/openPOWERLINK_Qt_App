/**
********************************************************************************
\file   SdoTransfer.h

\brief  SDO Transfer describes the individual ui frame for the SDO transer
		and its actions.

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

#ifndef _SDO_TRANSFER_H_
#define _SDO_TRANSFER_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "ui_SdoTransfer.h"
#include "user/SdoTransferResult.h"
#include "user/SdoTransferJob.h"

#include <QMetaMethod>
#include <QMap>

/**
 * \brief The SdoTransfer class prepares the ui frame and describes the
 * actions involved in it.
 */
class SdoTransfer : public QWidget
{
	Q_OBJECT

public:

	explicit SdoTransfer(QWidget *parent = 0);

public slots:

	void UpdateNodeList(unsigned int nodeId);

	void RemoveFromNodeList(unsigned int nodeId);

private slots:
	/**
	 * \brief Toggles the state of the value textbox based on the read/write.
	 * \param selected The state of the read radio button.
	 */
	void on_read_toggled(bool selected);

	/**
	 * \brief Executes the SDO transfer.
	 */
	void on_executeTransfer_clicked();

	/**
	 * \brief Sets the mask and validator depending on the selected datatype.
	 * \param dataType The selected datatype.
	 */
	void on_dataType_currentIndexChanged(const QString& dataType);

	/**
	 * \brief Validates the SDO write value once it is entered.
	 * \param[in] newValue The new value entered by the user.
	 */
	void on_sdoResultValue_textEdited(const QString& newValue);

	/**
	 * \brief Copies the contents of the log window.
	 */
	void on_copyLog_clicked();

	/**
	 * \brief Checks for if any text in the console is selected or not
	 * \param[in] txtSelected Checks if the text selected or not.
	 */
	void on_log_copyAvailable(bool txtSelected);

private:
	Ui::SdoTransfer ui;

	QVariant sdoTransferData;      ///< The transfer data.

	QValidator *sdoValueValidator; ///< Validator for the input value.
	quint64 maxDataValue;          ///< Maximum limit of value for the sdoTransferData.
	qint64 minDataValue;           ///< Minimum limit of value for the sdoTransferData.

	//TODO static qmap
	const QString sdoViaUdpStr;    ///< UDP String
	const QString sdoViaASndStr;   ///< ASnd String

	SdoTransferJob *sdoTransferJob;///< SDO Transfer Job instance.

	// receiver object should be a part of the class object.
	// Because the receiver function needs the objects memory while for RemoteSDO Transfer.
	//TODO static const
	QMetaMethod receiverFunction;  ///< MetaMethod object for the receiver Function

	QMetaType::Type metaDataTypeIndex; ///< The metatype of the Datatype selected.

	QStringList nodeIdList;        ///< Node id list.

	bool copyAvailable;            ///< Used to check if any of the log contents selected or not.

	/**
	 * \return a dataType map
	 */
	static QMap<QString, QMetaType::Type> CreateDataTypeMap();

	static const QMap<QString, QMetaType::Type> dataTypeMap; ///< Map to hold the datatype list

	/**
	 * \brief   Handles the SDO transfer result events from the remote SDO Transfer.
	 *
	 * \param[in] result The result of the SDO transfer
	 */
	Q_INVOKABLE void HandleSdoTransferFinished(const SdoTransferResult result);

	/**
	 * \brief Checks for the value is a valid value or not.
	 *
	 * It also updates the datatype of the sdoTransferData.
	 * \param[in] value The value to check.
	 * \retval true  If the value is valid.
	 * \retval false If the value is not-valid or if it doesn't falls within the selected range.
	 */
	bool IsValidValue(const QString& value);

	/**
	 * \brief Updates the SDO transfer value with the value in the SdoTransferData
	 * which has been used for the SDO Transfer.
	 */
	void UpdateSdoTransferReturnValue();

	/**
	 * \brief Sets the mask for the input value based on the selected datatype.
	 */
	void SetMaskForValue();

	/**
	 * \brief Sets the minimum and maximum value for the datatype selected.
	 * \param[in] datatype The datatype
	 */
	void SetMinMax(QMetaType::Type datatype);

	/**
	 * \brief Updates the log message to the SDO Transfer log with the date time.
	 * \param[in] logMessage The message to be updated.
	 */
	void UpdateLog(const QString& logMessage);
};

#endif // _SDO_TRANSFER_H_

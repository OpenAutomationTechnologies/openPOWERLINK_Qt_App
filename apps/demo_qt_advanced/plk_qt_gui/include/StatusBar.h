/**
********************************************************************************
\file   StatusBar.h

\brief  Refer StatusBar class

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
#ifndef _STATUSBAR_H_
#define _STATUSBAR_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <QStatusBar>
#include <QLabel>

#include <oplk/nmt.h>

/**
 * \brief The StatusBar class
 */
class StatusBar : public QStatusBar
{
	Q_OBJECT
public:
	explicit StatusBar(QWidget *parent = 0);

public slots:
	/**
	 * \brief Updates the NMT status of the MN on the status bar.
	 * \param[in] nmtState The state of the MN.
	 */
	void UpdateNmtStatus(tNmtState nmtState);

	/**
	 * \brief Sets the CDC file path to the status bar.
	 * \param[in] cdc Path to the CDC file.
	 */
	void SetCdcFilePath(QString& cdc);

	/**
	 * \brief Sets the Xap file path to the status bar.
	 * \param[in] xap Path to the Xap.xml file.
	 */
	void SetXapFilePath(QString& xap);

	/**
	 * \brief Sets the network interface name to the status bar.
	 * \param[in] name The name of the network interface.
	 */
	void SetNetworkInterfaceName(const QString& name);

	/**
	 * \brief HandleSyncWaitTimeChanged
	 * \param[in] sleepTime
	 */
	void HandleSyncWaitTimeChanged(ulong sleepTime);

private:
	QLabel *networkInterface;
	QLabel *cdcFile;
	QLabel *xapFile;
	QLabel *nmtStatus;
	QLabel *cycleTime;
	QLabel *refreshRate;

	/**
	 * \brief Reads and updates the cycle time on the status bar.
	 */
	void UpdateCycleTime();

	/**
	 * \param[in] path The full path string.
	 * \return The short path filled with ...
	 */
	const QString GetShortPath(const QString& path) const;
};

#endif // _STATUSBAR_H_

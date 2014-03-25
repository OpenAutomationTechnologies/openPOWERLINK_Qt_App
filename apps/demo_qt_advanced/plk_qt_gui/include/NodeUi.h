/**
********************************************************************************
\file   NodeUi.h

\brief  Inherits the QFrame and handles the status of the individual node.

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

#ifndef _NODE_UI_H_
#define _NODE_UI_H_

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>

#include <oplk/nmt.h>

/**
 * \brief The NodeUi class inherits the QFrame and updates the status of the node.
 */
class NodeUi : public QFrame
{
	Q_OBJECT

public:
	/**
	 * \brief Constructs a node status frame with the given nodeId.
	 * \param[in] nodeId node id for which the frame has been created.
	 * \param[in] parent
	 */
	explicit NodeUi(const UINT nodeId, QWidget *parent = 0);

	/**
	 * \brief  Handles the state changes of the corresponding node.
	 *
	 * Updates the state of the node with the respective colour
	 * and updates the tool tip.
	 *
	 * \param[in] nmtState The new state of the node.
	 */
	Q_INVOKABLE void HandleNodeStateChanged(tNmtState nmtState);

	/**
	 * \return The node id of the current instance.
	 */
	const UINT GetNodeId() const;

	//TODO Destructor

private:
	QHBoxLayout *nodeLayout; ///< The layout for the node frame.
	QLabel *name;            ///< A label to specify the name of the node.
	QLabel *statusImage;     ///< A label to display the status of the node.
	QPixmap statusPixmap;    ///< Pixmap to hold the image of the status.
	const UINT nodeId;       ///< The node id of this node frame.
};

#endif // _NODE_UI_H_

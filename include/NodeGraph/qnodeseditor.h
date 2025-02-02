/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef QNODESEDITOR_H
#define QNODESEDITOR_H

//----------------------------------------------------------------------------------------------------------------------
/// @class QNEBlock
/// @brief This class originally written by STANISLAW ADASZEWSKI has been modified
/// @brief to be used for a node based user interface for creating OSL shaders.
/// @date Modifed from 18/03/2015
/// @author STANISLAW ADASZEWSKI modified by Declan Russell
/// @brief [Accessed 2015]. availible from http://algoholic.eu/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
//----------------------------------------------------------------------------------------------------------------------

#include <QObject>

class QGraphicsScene;
class QNEConnection;
class QGraphicsItem;
class QPointF;
class QNEBlock;

class QNodesEditor : public QObject
{
	Q_OBJECT
public:
	explicit QNodesEditor(QObject *parent = 0);

	void install(QGraphicsScene *scene);

	bool eventFilter(QObject *, QEvent *);

	void save(QDataStream &ds);
	void load(QDataStream &ds);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief accesor to our scene
    //----------------------------------------------------------------------------------------------------------------------
    QGraphicsScene* getScene(){return scene;}
    //----------------------------------------------------------------------------------------------------------------------
signals:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief signal to notify if something in our material has changed
    //----------------------------------------------------------------------------------------------------------------------
    void matChanged();
public slots:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief slot to be called if the material has been changed
    //----------------------------------------------------------------------------------------------------------------------
    inline void signalMatChanged(){matChanged();}
    //----------------------------------------------------------------------------------------------------------------------
private:
	QGraphicsItem *itemAt(const QPointF&);

private:
	QGraphicsScene *scene;
	QNEConnection *conn;
	// QNEBlock *selBlock;
};

#endif // QNODESEDITOR_H

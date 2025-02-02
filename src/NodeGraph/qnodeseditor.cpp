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

#include "NodeGraph/qnodeseditor.h"

#include <QGraphicsScene>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <iostream>

#include "NodeGraph/qneport.h"
#include "NodeGraph/qneconnection.h"
#include "NodeGraph/qneblock.h"
#include "NodeGraph/OSLAbstractVarBlock.h"
#include "NodeGraph/OSLAbstractVarBlock.h"
#include "NodeGraph/OSLVarColorBlock.h"
#include "NodeGraph/OSLVarFloatBlock.h"
#include "NodeGraph/OSLVarFloatThreeBlock.h"
#include "NodeGraph/OSLVarImageBlock.h"
#include "NodeGraph/OSLVarIntBlock.h"
#include "NodeGraph/OSLVarNormalBlock.h"
#include "NodeGraph/OSLVarPointBlock.h"
#include "NodeGraph/OSLShaderBlock.h"
#include "UI/AbstractMaterialWidget.h"

QNodesEditor::QNodesEditor(QObject *parent) :
    QObject(parent)
{
	conn = 0;
}

void QNodesEditor::install(QGraphicsScene *s)
{
	s->installEventFilter(this);
	scene = s;
}

QGraphicsItem* QNodesEditor::itemAt(const QPointF &pos)
{
    QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

	foreach(QGraphicsItem *item, items)
		if (item->type() > QGraphicsItem::UserType)
			return item;

	return 0;
}

bool QNodesEditor::eventFilter(QObject *o, QEvent *e)
{
	QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*) e;

	switch ((int) e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	{

		switch ((int) me->button())
		{
		case Qt::LeftButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && item->type() == QNEPort::Type)
			{
                conn = new QNEConnection(0);
                scene->addItem(conn);
				conn->setPort1((QNEPort*) item);
				conn->setPos1(item->scenePos());
				conn->setPos2(me->scenePos());
				conn->updatePath();

				return true;
//			} else if (item && item->type() == QNEBlock::Type)
//			{
//				 if (selBlock)
//					selBlock->setSelected();
//				 selBlock = (QNEBlock*) item;
            }
			break;
		}
        case Qt::MidButton:
        {
            QGraphicsItem *item = itemAt(me->scenePos());
            if (item && (item->type() > QGraphicsItem::UserType) && (item->type()!= QNEPort::Type )){
                std::cerr<<"Deleting Scene Item"<<std::endl;
                scene->removeItem(item);
                delete item;
            }
			// if (selBlock == (QNEBlock*) item)
				// selBlock = 0;
			break;
		}
		}
	}
	case QEvent::GraphicsSceneMouseMove:
	{
		if (conn)
		{
			conn->setPos2(me->scenePos());
			conn->updatePath();
			return true;
		}
		break;
	}
	case QEvent::GraphicsSceneMouseRelease:
	{
		if (conn && me->button() == Qt::LeftButton)
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && item->type() == QNEPort::Type)
			{
				QNEPort *port1 = conn->port1();
				QNEPort *port2 = (QNEPort*) item;

                //modified to also check that the type is the same as we dont want to conflict osl standards
                //set sets the variable that the node is connected to
                if (port1->block() != port2->block() && port1->isOutput() != port2->isOutput() && !port1->isConnected(port2) && port1->getVaribleType() == port2->getVaribleType())
				{
					conn->setPos2(port2->scenePos());
					conn->setPort2(port2);
					conn->updatePath();
					conn = 0;

                    if (port1->block()->type()==OSLAbstractVarBlock::Type)
                    {
                        ((OSLAbstractVarBlock *)port1->block())->setLinkedVar();
                    }
                    else if (port2->block()->type()==OSLAbstractVarBlock::Type)
                    {
                        ((OSLAbstractVarBlock *)port2->block())->setLinkedVar();
                    }
					return true;
				}
			}

			delete conn;
			conn = 0;
			return true;
		}
		break;
	}
	}
	return QObject::eventFilter(o, e);
}

void QNodesEditor::save(QDataStream &ds)
{
    foreach(QGraphicsItem *item, scene->items()){
        if (item->type() == QNEBlock::Type || item->type() == OSLAbstractVarBlock::Type)
		{
			ds << item->type();
			((QNEBlock*) item)->save(ds);
        }
        else if(item->type() == OSLShaderBlock::Type){
            ds << item->type();
            ((OSLShaderBlock*) item)->save(ds);
        }
    }

	foreach(QGraphicsItem *item, scene->items())
        if (item->type() == QNEConnection::Type)
		{
			ds << item->type();
			((QNEConnection*) item)->save(ds);
		}
}

void QNodesEditor::load(QDataStream &ds)
{
	scene->clear();

	QMap<quint64, QNEPort*> portMap;

	while (!ds.atEnd())
	{
		int type;
		ds >> type;
        if (type == QNEBlock::Type)
		{
            QNEBlock *block = new QNEBlock(0);
            scene->addItem(block);
			block->load(ds, portMap);
        }
        else if (type == QNEConnection::Type)
		{
            QNEConnection *conn = new QNEConnection(0);
            scene->addItem(conn);
			conn->load(ds, portMap);
		}
        else if (type == OSLShaderBlock::Type){
            OSLShaderBlock *sblock = new OSLShaderBlock();
            scene->addItem(sblock);
            sblock->load(ds, portMap);
        }
        else if (type == OSLAbstractVarBlock::Type){
            int varType;
            ds >> varType;
            switch(varType){
            case(QNEPort::TypeColour):
            {
                OSLVarColorBlock *cblock = new OSLVarColorBlock(getScene(),AbstractMaterialWidget::getInstance()->getMaterial());
                connect(cblock->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
                cblock->load(ds, portMap);
                break;
            }
            case(QNEPort::TypeFloat):
            {
                OSLVarFloatBlock *fblock = new OSLVarFloatBlock(getScene(),AbstractMaterialWidget::getInstance()->getMaterial());
                connect(fblock->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
                fblock->load(ds, portMap);
                break;
            }
            case(QNEPort::TypeVector):
            {
                OSLVarFloatThreeBlock *fblock = new OSLVarFloatThreeBlock(getScene(),AbstractMaterialWidget::getInstance()->getMaterial());
                connect(fblock->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
                fblock->load(ds, portMap);
                break;
            }
            case(QNEPort::TypeInt):
            {
                OSLVarIntBlock *iblock = new OSLVarIntBlock(getScene(),AbstractMaterialWidget::getInstance()->getMaterial());
                connect(iblock->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
                iblock->load(ds, portMap);
                break;
            }
            case(QNEPort::TypeNormal):
            {
                OSLVarNormalBlock *nblock = new OSLVarNormalBlock(getScene(),AbstractMaterialWidget::getInstance()->getMaterial());
                connect(nblock->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
                nblock->load(ds, portMap);
                break;
            }
            case(QNEPort::TypePoint):
            {
                OSLVarPointBlock *pblock = new OSLVarPointBlock(getScene(),AbstractMaterialWidget::getInstance()->getMaterial());
                connect(pblock->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
                pblock->load(ds, portMap);
                break;
            }
            case(QNEPort::TypeString):
            {
                OSLVarImageBlock *imblock = new OSLVarImageBlock(getScene(),AbstractMaterialWidget::getInstance()->getMaterial());
                connect(imblock->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
                imblock->load(ds, portMap);
                break;
            }
            }
        }
	}
}

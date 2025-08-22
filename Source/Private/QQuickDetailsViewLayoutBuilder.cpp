#include "QQuickDetailsViewLayoutBuilder.h"
#include "QQuickDetailsViewMananger.h"

QQuickDetailsViewHeaderRowBuilder::QQuickDetailsViewHeaderRowBuilder(QQuickItem* inRootItem)
	: mRootItem(inRootItem)
{
}

QPair<QQuickItem*, QQuickItem*> QQuickDetailsViewHeaderRowBuilder::makeNameValueSlot()
{
	QQmlEngine* engine = qmlEngine(mRootItem);
	QQmlContext* context = qmlContext(mRootItem);
	QQmlContext* newContext = new QQmlContext(context, mRootItem);
	QQmlComponent rootComp(newContext->engine());
	rootComp.setData(R"(
        import QtQuick;
        import QtQuick.Controls;
        Rectangle{
            id: topLevelRect  
            anchors.fill: parent
            color: hoverHandler.hovered ? "#33CE98FB" : "white"
            border.color: "#EEEEEE"  
            border.width: 0.5       
            Behavior on color {
                ColorAnimation { duration: 100 }
            }
            HoverHandler { id: hoverHandler }
            Image {
                id: indicator
                visible: detailsDelegate.isTreeNode && detailsDelegate.hasChildren
                x: padding + (detailsDelegate.depth * detailsDelegate.indent)
                anchors.verticalCenter: parent.verticalCenter
                mipmap: true 
                source: detailsDelegate.expanded 
                    ? "qrc:/Resources/Icon/expand.png" 
                    : "qrc:/Resources/Icon/unexpand.png"

                width: 12
                height: 12
            }
            Item{
                id: nameEditorContent
                height: parent.height
                anchors.left: parent.left 
                anchors.leftMargin: padding + (detailsDelegate.isTreeNode ? (detailsDelegate.depth + 1) * detailsDelegate.indent : 0)
                anchors.right: splitter.left
                anchors.verticalCenter: parent.verticalCenter
            }
            Item{
                id: valueEditorContent
                height: parent.height
                anchors.left: splitter.left 
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle{
                id: splitter
                width: 3
                height: parent.height
                x: detailsView.SpliterPencent * detailsView.width - 1  
    
                Rectangle{
                    id: visibleSplitter
                    width: 1
                    height: parent.height
                    color: "#DDDDDD"
                    anchors.horizontalCenter: parent.horizontalCenter  
                }
    
                MouseArea {
                    id: dragArea
                    hoverEnabled: true
                    cursorShape: containsMouse ? Qt.SplitHCursor : Qt.ArrowCursor
                    anchors.fill: parent  
                    drag.target: splitter  
                    drag.axis: Drag.XAxis
                    drag.minimumX: 10 + 1  
                    drag.maximumX: detailsView.width - 10 - 1  
                    onPositionChanged: {
                        detailsView.SpliterPencent = (splitter.x + 1) / detailsView.width
                    }
                }
            }
        }
    )", QUrl());
    QQuickItem* slotItem = qobject_cast<QQuickItem*>(rootComp.create(newContext));
    qDebug() << rootComp.errorString();
    slotItem->setParentItem(mRootItem);
    return { slotItem->childItems()[1] ,slotItem->childItems()[2] };
}

void QQuickDetailsViewHeaderRowBuilder::makePropertyHeader(QPropertyHandle* inHandle)
{
	QQmlEngine* engine = qmlEngine(mRootItem);
	QQmlContext* context = qmlContext(mRootItem);
    QPair<QQuickItem*, QQuickItem*> slotItem = makeNameValueSlot();
	QQuickItem* nameEditor = inHandle->createNameEditor(slotItem.first);
	QQuickItem* valueEditor = inHandle->createValueEditor(slotItem.second);
	context->parentContext()->setContextProperty("heightProxy", valueEditor ? valueEditor : nameEditor);
}

QQuickDetailsViewLayoutBuilder::QQuickDetailsViewLayoutBuilder(IDetailsViewRow* inRow)
	: mRow(inRow)
{
}

QQuickDetailsViewLayoutBuilder* QQuickDetailsViewLayoutBuilder::addCustomRow(QQuickItem* item)
{
    return nullptr;
}

void QQuickDetailsViewLayoutBuilder::addProperty(QPropertyHandle* inPropertyHandle)
{
	QSharedPointer<IDetailsViewRow> child(new QDetailsViewRow_Property(inPropertyHandle));
	mRow->addChild(child);
	child->attachChildren();
}

void QQuickDetailsViewLayoutBuilder::addObject(QObject* inObject)
{
	for (int i = 1; i < inObject->metaObject()->propertyCount(); i++) {
		QMetaProperty prop = inObject->metaObject()->property(i);
		QString propertyPath = prop.name();
		QPropertyHandle* handler = QPropertyHandle::FindOrCreate(inObject, propertyPath);
		if (handler)
			addProperty(handler);
		else
			qWarning() << "property handle is null";
	}
}

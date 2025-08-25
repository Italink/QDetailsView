#include "QQuickDetailsViewLayoutBuilder.h"
#include "QQuickDetailsViewMananger.h"

QQuickDetailsViewRowBuilder::QQuickDetailsViewRowBuilder(IDetailsViewRow* inRow, QQuickItem* inRootItem)
	: mRow(inRow)
    , mRootItem(inRootItem)
{
    setHeightProxy(nullptr);
}

QPair<QQuickItem*, QQuickItem*> QQuickDetailsViewRowBuilder::makeNameValueSlot()
{
	QQmlEngine* engine = qmlEngine(mRootItem);
	QQmlContext* context = qmlContext(mRootItem);
	QQmlContext* newContext = new QQmlContext(context, mRootItem);
	QQmlComponent rootComp(newContext->engine());
	rootComp.setData(R"(
        import QtQuick;
        import QtQuick.Controls;
        import Qt5Compat.GraphicalEffects
        import ColorPalette
        Rectangle{
            id: topLevelRect  
            anchors.fill: parent
            color: hoverHandler.hovered ? ColorPalette.theme.rowBackgroundHover : ColorPalette.theme.rowBackground
            border.color: ColorPalette.theme.rowBorder
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
                ColorOverlay {
                    anchors.fill: parent
                    source: parent
                    color: ColorPalette.theme.rowIndicator  
                    opacity: 1.0 
                }
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
                implicitHeight: 25
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
                color : ColorPalette.theme.rowBackground
                x: detailsView.SpliterPencent * detailsView.width - 1  
    
                Rectangle{
                    id: visibleSplitter
                    width: 1
                    height: parent.height
                    color: ColorPalette.theme.rowSplitter
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
            Rectangle {
                id: gradientBox
                visible: detailsDelegate.depth > 0  
                width: 5
                x: padding + (detailsDelegate.depth * detailsDelegate.indent)
                height: parent.height
                gradient: Gradient {
                    orientation: Gradient.Horizontal 
                    GradientStop { position: 0.0; color: ColorPalette.theme.rowShadowStart }  
                    GradientStop { position: 1.0; color: ColorPalette.theme.rowShadowEnd } 
                }
            }
        }
    )", QUrl());
    QQuickItem* slotItem = qobject_cast<QQuickItem*>(rootComp.create(newContext));
	if (!rootComp.errors().isEmpty()) {
		qDebug() << rootComp.errorString();
	}
    slotItem->setParentItem(mRootItem);
    return { slotItem->childItems()[1] ,slotItem->childItems()[2] };
}

QQuickItem* QQuickDetailsViewRowBuilder::rootItem()
{
    return mRootItem;
}

QQuickItem* QQuickDetailsViewRowBuilder::setupItem(QQuickItem* inParent, QString inQmlCode)
{
	QQmlEngine* engine = qmlEngine(inParent);
	QQmlContext* context = qmlContext(inParent);
	QQmlComponent comp(engine);
	QQmlComponent nameComp(engine);
	comp.setData(inQmlCode.toLocal8Bit(), QUrl());
	QVariantMap initialProperties;
	initialProperties["parent"] = QVariant::fromValue(inParent);
	auto item = qobject_cast<QQuickItem*>(comp.createWithInitialProperties(initialProperties, context));
    if (!comp.errors().isEmpty()) {
        qDebug() << comp.errorString();
    }
	item->setParentItem(inParent);
    return item;
}

void QQuickDetailsViewRowBuilder::setupLabel(QQuickItem* inParent, QString inText)
{
    QQuickItem* lableItem = setupItem(inParent, R"(
	    import QtQuick;
	    import QtQuick.Controls;
	    import ColorPalette;
	    Item{
            property string lableText
		    implicitHeight: 25
		    width: parent.width
		    anchors.verticalCenter: parent.verticalCenter
		    Text {
			    anchors.fill: parent
			    verticalAlignment: Text.AlignVCenter
			    clip: true
			    elide: Text.ElideRight
			    text: lableText
			    color: ColorPalette.theme.labelPrimary
		    }
	    }
    )");
    lableItem->setProperty("lableText", inText);
}

void QQuickDetailsViewRowBuilder::setHeightProxy(QQuickItem* inProxyItem)
{
	QQmlEngine* engine = qmlEngine(mRootItem);
	QQmlContext* context = qmlContext(mRootItem);
    context->parentContext()->setContextProperty("heightProxy", inProxyItem);
}

void QQuickDetailsViewRowBuilder::makePropertyRow(QPropertyHandle* inHandle)
{
	QQmlEngine* engine = qmlEngine(mRootItem);
	QQmlContext* context = qmlContext(mRootItem);
    QPair<QQuickItem*, QQuickItem*> slotItem = makeNameValueSlot();
	QQuickItem* nameEditor = inHandle->createNameEditor(slotItem.first);
	QQuickItem* valueEditor = inHandle->createValueEditor(slotItem.second);
	context->parentContext()->setContextProperty("heightProxy", valueEditor ? valueEditor : nameEditor);
}

QQuickDetailsViewLayoutBuilder::QQuickDetailsViewLayoutBuilder(IDetailsViewRow* inRow)
	: mRootRow(inRow)
{
}

void QQuickDetailsViewLayoutBuilder::addCustomRow(std::function<void(QQuickDetailsViewRowBuilder*)> creator)
{
	QSharedPointer<IDetailsViewRow> child(new QDetailsViewRow_Custom(creator));
	mRootRow->addChild(child);
	child->attachChildren();
}

void QQuickDetailsViewLayoutBuilder::addProperty(QPropertyHandle* inPropertyHandle)
{
	QSharedPointer<IDetailsViewRow> child(new QDetailsViewRow_Property(inPropertyHandle));
	mRootRow->addChild(child);
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

#include "QDetailsView.h"
#include <QVBoxLayout>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QUrl>

QDetailsView::QDetailsView(QWidget* parent) 
	: QWidget(parent)
	, mQuickWidget(nullptr)
	, mQuickDetailsView(nullptr)
{
	setMinimumSize(200, 200);
	qmlRegisterType<QQuickDetailsView>("QtQuick.DetailsView", 1, 0, "DetailsView");

	qmlRegisterSingletonType(QUrl("qrc:/Resources/Qml/ColorPalette/ColorPalette.qml"),
		"ColorPalette", 
		1, 0,               
		"ColorPalette");     

	qmlRegisterSingletonType(QUrl("qrc:/Resources/Qml/ColorPalette/ColorPalette_Light.qml"),
		"ColorPalette", 
		1, 0,              
		"ColorPalette_Light"); 

	qmlRegisterSingletonType(QUrl("qrc:/Resources/Qml/ColorPalette/ColorPalette_Dark.qml"),
		"ColorPalette",
		1, 0,
		"ColorPalette_Dark");

	mQuickWidget = new QQuickWidget(this);
	mQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

	const QString qmlContent = R"(
        import QtQuick 
        import QtQuick.DetailsView 

        DetailsView {
            anchors.fill: parent
			boundsBehavior: Flickable.OvershootBounds
        }
    )";

	QQmlComponent component(mQuickWidget->engine());
	component.setData(qmlContent.toUtf8(), QUrl(""));

	QObject* rootObject = component.create();
	if (rootObject) {
		mQuickWidget->setSource(QUrl());
		mQuickWidget->engine()->setObjectOwnership(rootObject, QQmlEngine::CppOwnership);

		QQuickItem* rootItem = qobject_cast<QQuickItem*>(rootObject);
		if (rootItem) {
			mQuickWidget->setContent(QUrl(), &component, rootItem);
			mQuickDetailsView = qobject_cast<QQuickDetailsView*>(rootItem);
		}
	}

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(mQuickWidget);
	setLayout(layout);
}

QQuickDetailsView* QDetailsView::getQuickDetailsView() const
{
	return mQuickDetailsView;
}

void QDetailsView::setObject(QObject* inObject)
{
	QQuickItem* rootObject = mQuickWidget->rootObject();
	if (rootObject) {
		rootObject->setProperty("Object", QVariant::fromValue(inObject));
	}
}

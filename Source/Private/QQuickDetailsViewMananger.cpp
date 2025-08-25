#include "QQuickDetailsViewMananger.h"
#include "QPropertyHandle.h"
#include "QQuickFunctionLibrary.h"
#include <QRegularExpression>
#include "QQuickDetailsView.h"
#include "Customization/PropertyTypeCustomization_Sequential.h"
#include "Customization/PropertyTypeCustomization_Associative.h"
#include "Customization/PropertyTypeCustomization_ObjectDefault.h"
#include "Customization/PropertyTypeCustomization_Matrix4x4.h"
#include <QtQuickControls2/QQuickStyle>
#include <QMatrix4x4>

QQuickDetailsViewManager* QQuickDetailsViewManager::Get()
{
	static QQuickDetailsViewManager ins;
	return &ins;
}

void QQuickDetailsViewManager::initialize()
{
	mInitialized = true;

#ifdef QDETAILS_VIEW_STATIC_LIBRARY
	Q_INIT_RESOURCE(Resources);
#endif

	QQuickStyle::setStyle("Basic");

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
}

bool QQuickDetailsViewManager::isInitialized() const
{
	return mInitialized;
}

void QQuickDetailsViewManager::registerQml()
{
	qmlRegisterType<QQuickDetailsView>("Qt.DetailsView", 1, 0, "DetailsView");
}

void QQuickDetailsViewManager::unregisterCustomClassLayout(const QMetaObject* InMetaObject)
{
	mCustomClassTypeLayoutMap.remove(InMetaObject);
}

void QQuickDetailsViewManager::unregisterCustomPropertyTypeLayout(const QMetaType& InMetaType)
{
	mCustomPropertyTypeLayoutMap.remove(InMetaType);
}

void QQuickDetailsViewManager::registerCustomPropertyValueEditorCreator(const QMetaType& inMetaType, CustomPropertyValueWidgetCreator Creator)
{
	mPropertyValueEditorCreatorMap.insert(inMetaType, Creator);
}

void QQuickDetailsViewManager::unregisterCustomPropertyValueEditorCreator(const QMetaType& inMetaType)
{
	mPropertyValueEditorCreatorMap.remove(inMetaType);
}

QQuickItem* QQuickDetailsViewManager::createValueEditor(QPropertyHandle* inHandle, QQuickItem* parent)
{
	if (mPropertyValueEditorCreatorMap.contains(inHandle->getType())) {
		QQuickItem* item = mPropertyValueEditorCreatorMap[inHandle->getType()](inHandle, parent);
		if (parent) {
			item->setProperty("anchors.verticalCenter", QVariant::fromValue(parent->property("anchors.verticalCenter")));
			//item->update();
		}
		return item;
	}
	return nullptr;
}

QSharedPointer<IPropertyTypeCustomization> QQuickDetailsViewManager::getCustomPropertyType(QPropertyHandle* inHandle)
{
	if (inHandle->getPropertyType() == QPropertyHandle::Sequential) {
		return QSharedPointer<PropertyTypeCustomization_Sequential>::create();
	}
	else if (inHandle->getPropertyType() == QPropertyHandle::Associative) {
		return QSharedPointer<PropertyTypeCustomization_Associative>::create();
	}
	else if (inHandle->getPropertyType() == QPropertyHandle::Object) {
		const QMetaObject* metaObject = inHandle->metaObject();
		for (const auto& It : mCustomClassTypeLayoutMap.asKeyValueRange()) {
			if (It.first == metaObject) {
				return It.second();
			}
		}
		for (const auto& It : mCustomClassTypeLayoutMap.asKeyValueRange()) {
			if (metaObject->inherits(It.first)) {
				return It.second();
			}
		}
		return QSharedPointer<PropertyTypeCustomization_ObjectDefault>::create();
	}
	else if (inHandle->getPropertyType() == QPropertyHandle::RawType) {
		const QMetaType& metaType = inHandle->getType();
		for (const auto& It : mCustomPropertyTypeLayoutMap.asKeyValueRange()) {
			if (It.first == metaType) {
				return It.second();
			}
		}
		const QMetaObject* Child = nullptr;
		QRegularExpression reg("QSharedPointer\\<(.+)\\>");
		QRegularExpressionMatch match = reg.match(metaType.name());
		QStringList matchTexts = match.capturedTexts();
		QMetaType innerMetaType;
		if (!matchTexts.isEmpty()) {
			innerMetaType = QMetaType::fromName((matchTexts.back()).toLocal8Bit());
			Child = innerMetaType.metaObject();
		}
		else {
			Child = metaType.metaObject();
		}
		for (const auto& It : mCustomPropertyTypeLayoutMap.asKeyValueRange()) {
			const QMetaObject* Parent = It.first.metaObject();
			if (Parent && Child && Child->inherits(Parent)) {
				return It.second();
			}
		}
	}
	return nullptr;
}

QQuickDetailsViewManager::QQuickDetailsViewManager()
{
	RegisterBasicTypeEditor();

	registerCustomPropertyTypeLayout<QMatrix4x4, PropertyTypeCustomization_Matrix4x4>();
}


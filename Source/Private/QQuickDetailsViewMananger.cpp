#include "QQuickDetailsViewMananger.h"
#include "QPropertyHandle.h"
#include "QQuickFunctionLibrary.h"
#include <QRegularExpression>
#include "QQuickDetailsView.h"

QQuickDetailsViewManager* QQuickDetailsViewManager::Get()
{
	static QQuickDetailsViewManager ins;
	return &ins;
}

void QQuickDetailsViewManager::registerQml()
{
	qmlRegisterType<QQuickDetailsView>("Qt.DetailsView", 1, 0, "DetailsView");
}

void QQuickDetailsViewManager::unregisterCustomClassLayout(const QMetaObject* InMetaObject)
{
	mCustomClassLayoutMap.remove(InMetaObject);
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
			item->update();
		}
		return item;
	}
	return nullptr;
}

QSharedPointer<IDetailCustomization> QQuickDetailsViewManager::getCustomDetailLayout(const QMetaObject* InMetaObject)
{
	for (const auto& It : mCustomClassLayoutMap.asKeyValueRange()) {
		if (It.first == InMetaObject) {
			return It.second();
		}
	}
	for (const auto& It : mCustomClassLayoutMap.asKeyValueRange()) {
		if (InMetaObject->inherits(It.first)) {
			return It.second();
		}
	}
	return nullptr;
}

QSharedPointer<IPropertyTypeCustomization> QQuickDetailsViewManager::getCustomPropertyType(const QMetaType& InMetaType)
{
	for (const auto& It : mCustomPropertyTypeLayoutMap.asKeyValueRange()) {
		if (It.first == InMetaType) {
			return It.second();
		}
	}
	const QMetaObject* Child = nullptr;
	QRegularExpression reg("QSharedPointer\\<(.+)\\>");
	QRegularExpressionMatch match = reg.match(InMetaType.name());
	QStringList matchTexts = match.capturedTexts();
	QMetaType innerMetaType;
	if (!matchTexts.isEmpty()) {
		innerMetaType = QMetaType::fromName((matchTexts.back()).toLocal8Bit());
		Child = innerMetaType.metaObject();
	}
	else {
		Child = InMetaType.metaObject();
	}

	for (const auto& It : mCustomPropertyTypeLayoutMap.asKeyValueRange()) {
		const QMetaObject* Parent = It.first.metaObject();
		if (Parent && Child && Child->inherits(Parent)) {
			return It.second();
		}
	}
	return nullptr;
}

#define REGINTER_NUMBER_EDITOR_CREATOR(TypeName,DefaultPrecision) \
	registerCustomPropertyValueEditorCreator(QMetaType::fromType<TypeName>(), [](QPropertyHandle* handle, QQuickItem* parent)->QQuickItem* { \
		QQmlEngine* engine = qmlEngine(parent);\
		QQmlContext* context = qmlContext(parent);\
		QQmlComponent nameComp(engine);\
		nameComp.setData(R"(
					import QtQuick;
					import QtQuick.Controls;
					import "qrc:/Resources/Qml"
					NumberBox{
						anchors.verticalCenter: parent.verticalCenter
						width: parent.width
					}
				)", QUrl());\
		QVariantMap initialProperties; \
		initialProperties["parent"] = QVariant::fromValue(parent);\
		auto valueEditor = qobject_cast<QQuickItem*>(nameComp.createWithInitialProperties(initialProperties, context));\
		valueEditor->setParentItem(parent);\
		TypeName min = handle->getMetaData("Min").toDouble();\
		TypeName max = handle->getMetaData("Max").toDouble();\
		double step = handle->getMetaData("Step").toDouble();\
		int precision = handle->getMetaData("Precision").toInt(); \
		valueEditor->setProperty("step", step);\
		valueEditor->setProperty("number", handle->getVar());\
		valueEditor->setProperty("precision", precision == 0 ? DefaultPrecision:precision); \
		if (min < max) {\
			if(step <= 0.000001) valueEditor->setProperty("step", (max - min) / 1000); \
			valueEditor->setProperty("min", min);\
			valueEditor->setProperty("max", max);\
			valueEditor->setProperty("isLimited", true);\
		}\
		connect(valueEditor, SIGNAL(valueChanged(QVariant)), handle, SLOT(setVar(QVariant))); \
		connect(handle, SIGNAL(asRequestRollback(QVariant)), valueEditor, SLOT(setNumber(QVariant))); \
		return valueEditor;\
	});

QQuickDetailsViewManager::QQuickDetailsViewManager()
{
	RegisterBasicTypeEditor();
}


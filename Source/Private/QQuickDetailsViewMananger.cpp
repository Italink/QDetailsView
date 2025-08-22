#include "QQuickDetailsViewMananger.h"
#include "QPropertyHandle.h"
#include "QQuickFunctionLibrary.h"
#include <QRegularExpression>
#include "QQuickDetailsView.h"
#include "Customization/ClassLayoutCustomization_Default.h"

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

QSharedPointer<IClassLayoutCustomization> QQuickDetailsViewManager::getCustomDetailLayout(const QMetaObject* InMetaObject)
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
	return QSharedPointer<ClassLayoutCustomization_Default>::create();
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

QQuickDetailsViewManager::QQuickDetailsViewManager()
{
	RegisterBasicTypeEditor();
}


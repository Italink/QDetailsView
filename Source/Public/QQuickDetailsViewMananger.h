#ifndef QQuickDetailsViewManager_h__
#define QQuickDetailsViewManager_h__

#include <QObject>
#include <QHash>
#include <functional>
#include <QMetaType>
#include <QQmlEngine>
#include <QQuickItem>
#include "IPropertyTypeCustomization.h"
#include "QDetailsViewAPI.h"

class QPropertyHandle;

class QDETAILS_VIEW_API QQuickDetailsViewManager : public QObject{
public:
	using CustomPropertyTypeLayoutCreator = std::function<QSharedPointer<IPropertyTypeCustomization>()>;
	using CustomPropertyValueWidgetCreator = std::function<QQuickItem* (QPropertyHandle*, QQuickItem*)>;

	static QQuickDetailsViewManager* Get();

	void registerQml();

	template<typename IPropertyTypeCustomizationType>
	void registerCustomClassLayout(const QMetaObject* InMetaObject)
	{
		mCustomClassTypeLayoutMap.insert(InMetaObject, []() {
			return QSharedPointer<IPropertyTypeCustomizationType>::create();
		});
	}
	void unregisterCustomClassLayout(const QMetaObject* InMetaObject);

	template<typename MetaType, typename IPropertyTypeCustomizationType>
	void registerCustomPropertyTypeLayout() {
		mCustomPropertyTypeLayoutMap.insert(QMetaType::fromType<MetaType>(), []() {
			return QSharedPointer<IPropertyTypeCustomizationType>::create();
			});
	}
	void unregisterCustomPropertyTypeLayout(const QMetaType& InMetaType);

	void registerCustomPropertyValueEditorCreator(const QMetaType& inMetaType, CustomPropertyValueWidgetCreator Creator);
	void unregisterCustomPropertyValueEditorCreator(const QMetaType& inMetaType);

	QQuickItem* createValueEditor(QPropertyHandle* inHandle, QQuickItem* parent);
	QSharedPointer<IPropertyTypeCustomization> getCustomPropertyType(QPropertyHandle* inHandle);
protected:
	QQuickDetailsViewManager();
	void RegisterBasicTypeEditor();
private:
	QHash<const QMetaObject*, CustomPropertyTypeLayoutCreator> mCustomClassTypeLayoutMap;
	QHash<QMetaType, CustomPropertyTypeLayoutCreator> mCustomPropertyTypeLayoutMap;
	QHash<QMetaType, CustomPropertyValueWidgetCreator> mPropertyValueEditorCreatorMap;
};

#endif // QQuickDetailsViewManager_h__

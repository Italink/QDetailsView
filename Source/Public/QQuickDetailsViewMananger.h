#ifndef QQuickDetailsViewManager_h__
#define QQuickDetailsViewManager_h__

#include <QObject>
#include <QHash>
#include <functional>
#include <QMetaType>
#include <QQmlEngine>
#include <QQuickItem>
#include "IClassLayoutCustomization.h"
#include "IPropertyTypeCustomization.h"
#include "QDetailsViewAPI.h"

class QPropertyHandle;

class QDETAILS_VIEW_API QQuickDetailsViewManager : public QObject{
public:
	using CustomClassLayoutCreator = std::function<QSharedPointer<IClassLayoutCustomization>()>;
	using CustomPropertyTypeLayoutCreator = std::function<QSharedPointer<IPropertyTypeCustomization>()>;
	using CustomPropertyValueWidgetCreator = std::function<QQuickItem* (QPropertyHandle*, QQuickItem*)>;

	static QQuickDetailsViewManager* Get();

	void registerQml();

	template<typename IClassLayoutCustomizationType>
	void registerCustomClassLayout(const QMetaObject* InMetaObject)
	{
		mCustomClassLayoutMap.insert(InMetaObject, []() {
			return QSharedPointer<IClassLayoutCustomizationType>::create();
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
	QSharedPointer<IClassLayoutCustomization> getCustomDetailLayout(const QMetaObject* InMetaObject);
	QSharedPointer<IPropertyTypeCustomization> getCustomPropertyType(const QMetaType& InMetaType);
protected:
	QQuickDetailsViewManager();
	void RegisterBasicTypeEditor();
private:
	QHash<const QMetaObject*, CustomClassLayoutCreator> mCustomClassLayoutMap;
	QHash<QMetaType, CustomPropertyTypeLayoutCreator  > mCustomPropertyTypeLayoutMap;
	QHash<QMetaType, CustomPropertyValueWidgetCreator> mPropertyValueEditorCreatorMap;
};

#endif // QQuickDetailsViewManager_h__

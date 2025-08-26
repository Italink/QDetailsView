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
	using PropertyTypeCustomizationCreator = std::function<QSharedPointer<IPropertyTypeCustomization>()>;
	using TypeEditorCreator = std::function<QQuickItem* (QPropertyHandle*, QQuickItem*)>;

	static QQuickDetailsViewManager* Get();

	void initialize();
	bool isInitialized() const;

	template<typename MetaType, typename IPropertyTypeCustomizationType>
	void registerPropertyTypeCustomization() {
		QMetaType metaType = QMetaType::fromType<MetaType>();
		if (metaType.metaObject()) {
			mClassCustomizationMap.insert(metaType.metaObject(), []() {
				return QSharedPointer<IPropertyTypeCustomizationType>::create();
			});
		}
		else {
			mMetaTypeCustomizationMap.insert(metaType, []() {
				return QSharedPointer<IPropertyTypeCustomizationType>::create();
			});
		}

	}
	void unregisterPropertyTypeCustomization(const QMetaType& inMetaType);

	void registerTypeEditor(const QMetaType& inMetaType, TypeEditorCreator Creator);
	void unregisterTypeEditor(const QMetaType& inMetaType);

	QQuickItem* createValueEditor(QPropertyHandle* inHandle, QQuickItem* parent);
	QSharedPointer<IPropertyTypeCustomization> getCustomPropertyType(QPropertyHandle* inHandle);
protected:
	QQuickDetailsViewManager();
	void RegisterBasicTypeEditor();
private:
	bool mInitialized = false;

	QHash<const QMetaObject*, PropertyTypeCustomizationCreator> mClassCustomizationMap;
	QHash<QMetaType, PropertyTypeCustomizationCreator> mMetaTypeCustomizationMap;
	QHash<QMetaType, TypeEditorCreator> mTypeEditorCreatorMap;
};

#endif // QQuickDetailsViewManager_h__

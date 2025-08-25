#include "PropertyTypeCustomization_ObjectDefault.h"
#include "QQuickDetailsViewLayoutBuilder.h"
#include "QPropertyHandle.h"

void PropertyTypeCustomization_ObjectDefault::customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder)
{
	auto objectHandle = inPropertyHandle->asObject();
	const QMetaObject* metaObject = objectHandle->getMetaObject();
	for (int i = objectHandle->isGadget() ? 0 : 1; i < metaObject->propertyCount(); i++) {
		QMetaProperty prop = metaObject->property(i);
		QString propName = prop.name();
		if (objectHandle->isGadget()) {
			inBuilder->addProperty(inPropertyHandle->findOrCreateChild(
				prop.metaType(),
				propName,
				[this, prop, objectHandle]() {
					return prop.readOnGadget(objectHandle->getGadget());
				},
				[this, prop, objectHandle, inPropertyHandle](QVariant var) {
					prop.writeOnGadget(objectHandle->getGadget(), var);
					inPropertyHandle->setVar(objectHandle->getObjectHolder());
					objectHandle->refreshObjectPtr();
				}
			));
		}
		else {
			inBuilder->addProperty(inPropertyHandle->findOrCreateChild(
				prop.metaType(),
				propName,
				[this, prop, objectHandle]() {
					return prop.read(objectHandle->getObject());
				},
				[this, prop, objectHandle, inPropertyHandle](QVariant var) {
					prop.write(objectHandle->getObject(), var);
				}
			));
		}
	}
}


#include "PropertyTypeCustomization_ObjectDefault.h"
#include "QQuickDetailsViewLayoutBuilder.h"
#include "QPropertyHandle.h"

void PropertyTypeCustomization_ObjectDefault::customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder)
{
	auto ObjectHandle = inPropertyHandle->asObject();
	const QMetaObject* metaObject = ObjectHandle->getMetaObject();
	for (int i = ObjectHandle->isGadget() ? 0 : 1; i < metaObject->propertyCount(); i++) {
		QMetaProperty prop = metaObject->property(i);
		QString propName = prop.name();
		QPropertyHandle* handler = inPropertyHandle->findOrCreateChildHandle(propName);
		inBuilder->addProperty(handler);
	}
}


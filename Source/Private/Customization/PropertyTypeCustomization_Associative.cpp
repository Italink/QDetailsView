#include "PropertyTypeCustomization_Associative.h"
#include "QQuickDetailsViewLayoutBuilder.h"
#include "QPropertyHandle.h"
#include "qsequentialiterable.h"
#include "qassociativeiterable.h"

void PropertyTypeCustomization_Associative::customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder)
{
	auto associativelHandle = inPropertyHandle->asAssociative();
	QMetaAssociation metaAssociation = associativelHandle->metaAssociation();
	QVariant varMap = inPropertyHandle->getVar();
	QAssociativeIterable iterable = varMap.value<QAssociativeIterable>();
	for (auto iter = iterable.begin(); iter != iterable.end(); ++iter) {
		QString key = iter.key().toString();
		inBuilder->addProperty(inPropertyHandle->findOrCreateChild(
			metaAssociation.mappedMetaType(),
			key,
			[inPropertyHandle, key]() {
				QVariant varMap = inPropertyHandle->getVar();
				QAssociativeIterable iterable = varMap.value<QAssociativeIterable>();
				return iterable.value(key);
			},
			[inPropertyHandle, key, metaAssociation](QVariant var) {
				QVariant varMap = inPropertyHandle->getVar();
				QAssociativeIterable iterable = varMap.value<QAssociativeIterable>();
				QtPrivate::QVariantTypeCoercer keyCoercer;
				QtPrivate::QVariantTypeCoercer mappedCoercer;
				void* containterPtr = const_cast<void*>(iterable.constIterable());
				const void* dataPtr = mappedCoercer.coerce(var, var.metaType());
				metaAssociation.setMappedAtKey(containterPtr, keyCoercer.coerce(key, metaAssociation.keyMetaType()), dataPtr);
				inPropertyHandle->setVar(varMap);
			}
		));
	}
}
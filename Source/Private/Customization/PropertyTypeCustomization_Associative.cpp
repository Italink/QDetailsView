#include "PropertyTypeCustomization_Associative.h"
#include "QQuickDetailsViewLayoutBuilder.h"
#include "QPropertyHandle.h"
#include "qsequentialiterable.h"
#include "qassociativeiterable.h"

void PropertyTypeCustomization_Associative::customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder)
{
	QVariant varMap = inPropertyHandle->getVar();
	QAssociativeIterable iterable = varMap.value<QAssociativeIterable>();
	for (auto iter = iterable.begin(); iter != iterable.end(); ++iter) {
		QString name = iter.key().toString();
		QPropertyHandle* handle = inPropertyHandle->findOrCreateChildHandle(name);
		if (handle) {
			inBuilder->addProperty(handle);
		}
	}
}
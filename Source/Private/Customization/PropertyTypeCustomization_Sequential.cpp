#include "PropertyTypeCustomization_Sequential.h"
#include "QQuickDetailsViewLayoutBuilder.h"
#include "QPropertyHandle.h"
#include "qsequentialiterable.h"
#include "qassociativeiterable.h"

void PropertyTypeCustomization_Sequential::customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder)
{
	QVariant varList = inPropertyHandle->getVar();
	QSequentialIterable iterable = varList.value<QSequentialIterable>();
	for (int index = 0; index < iterable.size(); index++) {
		QString name = QString::number(index);
		QPropertyHandle* handle = inPropertyHandle->findOrCreateChildHandle(name);
		if (handle) {
			inBuilder->addProperty(handle);
		}
	}
}


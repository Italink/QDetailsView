#include "PropertyTypeCustomization_Sequential.h"
#include "QQuickDetailsViewLayoutBuilder.h"
#include "QPropertyHandle.h"
#include "qsequentialiterable.h"
#include "qassociativeiterable.h"

void PropertyTypeCustomization_Sequential::customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder)
{
	auto sequentialHandle = inPropertyHandle->asSequential();
	QVariant varList = inPropertyHandle->getVar();
	QSequentialIterable iterable = varList.value<QSequentialIterable>();
	for (int index = 0; index < iterable.size(); index++) {
		QString name = QString::number(index);
		inBuilder->addProperty(inPropertyHandle->findOrCreateChild(
			sequentialHandle->metaSequence().valueMetaType(),
			name,
			[inPropertyHandle, index]() {
				QVariant varList = inPropertyHandle->getVar();
				QSequentialIterable iterable = varList.value<QSequentialIterable>();
				if (index < 0 || index >= iterable.size()) {
					return QVariant();
				}
				return iterable.at(index);
			},
			[inPropertyHandle, index](QVariant var) {
				QVariant varList = inPropertyHandle->getVar();
				QSequentialIterable iterable = varList.value<QSequentialIterable>();
				if (index >= 0 && index < iterable.size()) {
					const QMetaSequence metaSequence = iterable.metaContainer();
					void* containterPtr = const_cast<void*>(iterable.constIterable());
					QtPrivate::QVariantTypeCoercer coercer;
					const void* dataPtr = coercer.coerce(var, var.metaType());
					metaSequence.setValueAtIndex(containterPtr, index, dataPtr);
					inPropertyHandle->setVar(varList);
				}
			}
		));
	}
}


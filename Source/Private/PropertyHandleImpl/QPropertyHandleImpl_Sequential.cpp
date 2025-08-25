#include "PropertyHandleImpl/QPropertyHandleImpl_Sequential.h"
#include <qsequentialiterable.h>
#include "QBoxLayout"
#include "QPropertyHandle.h"


QPropertyHandleImpl_Sequential::QPropertyHandleImpl_Sequential(QPropertyHandle* inHandle)
	:IPropertyHandleImpl(inHandle) {
	QVariant varList = mHandle->getVar();
	QSequentialIterable iterable = varList.value<QSequentialIterable>();
	mMetaSequence = iterable.metaContainer();
}

const QMetaSequence& QPropertyHandleImpl_Sequential::metaSequence() const
{
	return mMetaSequence;
}

QQuickItem* QPropertyHandleImpl_Sequential::createValueEditor(QQuickItem* inParent)
{
	return nullptr;
}

int QPropertyHandleImpl_Sequential::itemCount() {
	QVariant varList = mHandle->getVar();
	QSequentialIterable iterable = varList.value<QSequentialIterable>();
	return iterable.size();
}

void QPropertyHandleImpl_Sequential::appendItem( QVariant InVar) {
	QVariant varList = mHandle->getVar();
	QSequentialIterable iterable = varList.value<QSequentialIterable>();
	const QMetaSequence metaSequence = iterable.metaContainer();
	void* containterPtr = const_cast<void*>(iterable.constIterable());
	QtPrivate::QVariantTypeCoercer coercer;
	const void* dataPtr = coercer.coerce(InVar, InVar.metaType());
	metaSequence.addValue(containterPtr, dataPtr);
	//mHandle->setVar(varList, QString("%1 Append: %2").arg(mHandle->getPath()).arg(metaSequence.size(containterPtr) - 1));
	//Q_EMIT mHandle->asRequestRebuildRow();
}

void QPropertyHandleImpl_Sequential::moveItem(int InSrcIndex, int InDstIndex) {
	QVariant varList = mHandle->getVar();
	QSequentialIterable iterable = varList.value<QSequentialIterable>();
	const QMetaSequence metaSequence = iterable.metaContainer();
	void* containterPtr = const_cast<void*>(iterable.constIterable());
	QtPrivate::QVariantTypeCoercer coercer;
	QVariant srcVar = iterable.at(InSrcIndex);
	QVariant dstVar = iterable.at(InDstIndex);
	metaSequence.setValueAtIndex(containterPtr, InDstIndex, coercer.coerce(srcVar, srcVar.metaType()));
	metaSequence.setValueAtIndex(containterPtr, InSrcIndex, coercer.coerce(dstVar, dstVar.metaType()));
	//mHandle->setVar(varList, QString("%1 Move: %2->%3").arg(mHandle->getPath()).arg(InSrcIndex).arg(InDstIndex));
	//Q_EMIT mHandle->asRequestRebuildRow();
}

void QPropertyHandleImpl_Sequential::removeItem(int InIndex) {
	QVariant varList = mHandle->getVar();
	QSequentialIterable iterable = varList.value<QSequentialIterable>();
	const QMetaSequence metaSequence = iterable.metaContainer();
	void* containterPtr = const_cast<void*>(iterable.constIterable());
	QtPrivate::QVariantTypeCoercer coercer;
	for (int i = InIndex; i < iterable.size() - 1; i++) {
		QVariant nextVar = iterable.at(i + 1);
		metaSequence.setValueAtIndex(containterPtr, InIndex, coercer.coerce(nextVar, nextVar.metaType()));
	}
	metaSequence.removeValueAtEnd(containterPtr);
	//mHandle->setVar(varList, QString("%1 Remove: %2").arg(mHandle->getPath()).arg(InIndex));
	//Q_EMIT mHandle->asRequestRebuildRow();
}

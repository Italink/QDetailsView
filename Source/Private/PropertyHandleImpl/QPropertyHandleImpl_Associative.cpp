#include "PropertyHandleImpl/QPropertyHandleImpl_Associative.h"
#include <QAssociativeIterable>
#include "QBoxLayout"
#include "QPropertyHandle.h"

QPropertyHandleImpl_Associative::QPropertyHandleImpl_Associative(QPropertyHandle* inHandle)
	:IPropertyHandleImpl(inHandle) {
	QVariant varMap = mHandle->getVar();
	QAssociativeIterable iterable = varMap.value<QAssociativeIterable>();
	mMetaAssociation = iterable.metaContainer();
}

const QMetaAssociation& QPropertyHandleImpl_Associative::metaAssociation() const
{
	return mMetaAssociation;
}

QQuickItem* QPropertyHandleImpl_Associative::createValueEditor(QQuickItem* inParent)
{
	return nullptr;
}

bool QPropertyHandleImpl_Associative::renameItem(QString inSrc, QString inDst) {
	bool canRename = false;
	QVariant varMap = mHandle->getVar();
	QAssociativeIterable iterable = varMap.value<QAssociativeIterable>();
	if (iterable.containsKey(inSrc) && !iterable.containsKey(inDst)) {
		canRename = true;
		QVariant var = iterable.value(inSrc);
		QtPrivate::QVariantTypeCoercer keyCoercer;
		QtPrivate::QVariantTypeCoercer mappedCoercer;
		void* containterPtr = const_cast<void*>(iterable.constIterable());
		QMetaAssociation metaAssociation = iterable.metaContainer();
		metaAssociation.removeKey(containterPtr, keyCoercer.coerce(inSrc, QMetaType::fromType<QString>()));
		metaAssociation.setMappedAtKey(
			containterPtr,
			keyCoercer.coerce(inDst, QMetaType::fromType<QString>()),
			mappedCoercer.coerce(var, var.metaType())
		);
		//mHandle->setVar(varMap, QString("Rename: %1 -> %2").arg(inSrc).arg(inDst));
		//Q_EMIT mHandle->asRequestRebuildRow();
	}
	return canRename;
}

void QPropertyHandleImpl_Associative::appendItem(QString inKey, QVariant inValue) {
	QVariant varList = mHandle->getVar();
	QAssociativeIterable iterable = varList.value<QAssociativeIterable>();
	void* containterPtr = const_cast<void*>(iterable.constIterable());
	QtPrivate::QVariantTypeCoercer coercer;
	QVariant key(inKey);
	const void* keyDataPtr = coercer.coerce(key, key.metaType());
	const void* valueDataPtr = coercer.coerce(inValue, inValue.metaType());
	//metaAssociation.insertKey(containterPtr, keyDataPtr);
	mMetaAssociation.setMappedAtKey(containterPtr, keyDataPtr, valueDataPtr);
	//mHandle->setVar(varList, QString("%1 Insert: %2").arg(mHandle->getPath()).arg(inKey));
	//Q_EMIT mHandle->asRequestRebuildRow();
}

void QPropertyHandleImpl_Associative::removeItem(QString inKey) {
	QVariant varList = mHandle->getVar();
	QAssociativeIterable iterable = varList.value<QAssociativeIterable>();
	const QMetaAssociation metaAssociation = iterable.metaContainer();
	void* containterPtr = const_cast<void*>(iterable.constIterable());
	QtPrivate::QVariantTypeCoercer coercer;
	QVariant key(inKey);
	const void* keyDataPtr = coercer.coerce(key, key.metaType());
	metaAssociation.removeKey(containterPtr, keyDataPtr);
	//mHandle->setVar(varList, QString("%1 Remove: %2").arg(mHandle->getPath()).arg(inKey));
	//Q_EMIT mHandle->asRequestRebuildRow();
}


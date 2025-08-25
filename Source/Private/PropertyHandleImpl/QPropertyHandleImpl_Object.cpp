#include "PropertyHandleImpl/QPropertyHandleImpl_Object.h"
#include <qsequentialiterable.h>
#include <QRegularExpression>
#include "QPropertyHandle.h"
#include <QMetaProperty>

QPropertyHandleImpl_Object::QPropertyHandleImpl_Object(QPropertyHandle* inHandle)
	:IPropertyHandleImpl(inHandle) {
	mObjectHolder = mHandle->getVar();
	QMetaType metaType = mHandle->getType();
	QRegularExpression reg("QSharedPointer\\<(.+)\\>");
	QRegularExpressionMatch match = reg.match(metaType.name());
	QStringList matchTexts = match.capturedTexts();
	if (!matchTexts.isEmpty()) {
		QMetaType innerMetaType = QMetaType::fromName((matchTexts.back()).toLocal8Bit());
		mMetaObject = innerMetaType.metaObject();
		const void* ptr = *(const void**)mObjectHolder.data();
		bIsSharedPointer = true;
		bIsPointer = true;
		if (ptr) {
			mObjectHolder = QVariant(innerMetaType, mObjectHolder.data());
		}
		else {
			mObjectHolder = QVariant();
		}
	}
	else{
		bIsPointer = metaType.flags().testFlag(QMetaType::IsPointer);
		mMetaObject = metaType.metaObject();
	}
	mOwnerObject = mHandle->parent();
	refreshObjectPtr();
}

QObject* QPropertyHandleImpl_Object::getObject()
{
	if(mMetaObject->inherits(&QObject::staticMetaObject))
		return (QObject*)mObjectPtr;
	return nullptr;
}

void* QPropertyHandleImpl_Object::getGadget()
{
	return mObjectPtr;
}

bool QPropertyHandleImpl_Object::isGadget() const
{
	if (mMetaObject->inherits(&QObject::staticMetaObject))
		return false;
	return mObjectPtr != nullptr;
}

QObject* QPropertyHandleImpl_Object::getOwnerObject()
{
	return mOwnerObject;
}

const QMetaObject* QPropertyHandleImpl_Object::getMetaObject() const
{
	return mMetaObject;
}

void QPropertyHandleImpl_Object::refreshObjectPtr() {
	mObjectHolder = mHandle->getVar();
	if (mObjectHolder.isValid()) {
		if (mMetaObject->inherits(&QObject::staticMetaObject)) {
			QObject* objectPtr = mObjectHolder.value<QObject*>();
			if (objectPtr) {
				mMetaObject = objectPtr->metaObject();
			}
			mObjectPtr = objectPtr;
			mOwnerObject = objectPtr;
			if (mOwnerObject) {
				//QMetaObject::invokeMethod(mOwnerObject, std::bind(&QObject::moveToThread, mOwnerObject, mHandle->thread()));
				//QMetaObject::invokeMethod(mOwnerObject, std::bind(&QObject::installEventFilter, mOwnerObject, mHandle));
				//mOwnerObject->installEventFilter(mHandle);
			}
		}
		else {
			void* ptr = mObjectHolder.data();
			if (bIsPointer)
				ptr = *(void**)mObjectHolder.data();
			mObjectPtr = ptr;
		}
	}
}

QVariant& QPropertyHandleImpl_Object::getObjectHolder()
{
	return mObjectHolder;
}

QQuickItem* QPropertyHandleImpl_Object::createValueEditor(QQuickItem* inParent)
{
	return nullptr;
}


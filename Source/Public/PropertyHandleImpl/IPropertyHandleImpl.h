#ifndef IPropertyHandleImpl_h__
#define IPropertyHandleImpl_h__

#include <QQmlContext>
#include <QQuickItem>
#include <QVariant>
#include <QObject>
#include "QDetailsViewAPI.h"

class QPropertyHandle;

class IPropertyHandleImpl{
	friend class QPropertyHandle;
public:
	enum Type {
		Null,
		RawType,
		Associative,
		Sequential,
		Enum,
		Object,
	};
protected:
	IPropertyHandleImpl(QPropertyHandle* inHandle);
	virtual QPropertyHandle* findChildHandle(const QString& inSubName);
	virtual QPropertyHandle* findOrCreateChildHandle(const QString& inSubName);
	virtual QQuickItem* createNameEditor(QQuickItem* inParent);
	virtual QQuickItem* createValueEditor(QQuickItem* inParent)= 0;
	virtual Type type() { return Type::Null; };
protected:
	QPropertyHandle* mHandle;
};

#endif // IPropertyHandleImpl_h__

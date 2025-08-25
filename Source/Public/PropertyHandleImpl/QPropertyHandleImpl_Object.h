#ifndef QPropertyHandleImpl_Object_h__
#define QPropertyHandleImpl_Object_h__

#include "IPropertyHandleImpl.h"

class QDETAILS_VIEW_API QPropertyHandleImpl_Object : public IPropertyHandleImpl {
public:
	QPropertyHandleImpl_Object(QPropertyHandle* inHandle);
	QObject* getObject();
	void* getGadget();
	bool isGadget() const;
	QObject* getOwnerObject();
	const QMetaObject* getMetaObject() const;
protected:
	QPropertyHandle* findChildHandle(const QString& inSubName) override;
	QQuickItem* createValueEditor(QQuickItem* inParent)override;
	QPropertyHandle* findOrCreateChildHandle(const QString& inSubName) override;
	Type type() override { return Type::Object; };
	void refreshObjectPtr();
private:
	QVariant mObjectHolder;
	void* mObjectPtr = nullptr;
	QObject* mOwnerObject = nullptr;
	const QMetaObject* mMetaObject = nullptr;
	bool bIsSharedPointer = false;
	bool bIsPointer = false;
};

#endif // QPropertyHandleImpl_Object_h__

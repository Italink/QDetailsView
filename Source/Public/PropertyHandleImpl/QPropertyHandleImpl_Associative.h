#ifndef QAssociativePropertyHandle_h__
#define QAssociativePropertyHandle_h__

#include "QMetaContainer"
#include "IPropertyHandleImpl.h"

class QDETAILS_VIEW_API QPropertyHandleImpl_Associative: public IPropertyHandleImpl {
public:
	QPropertyHandleImpl_Associative(QPropertyHandle* inHandle);

	void appendItem(QString inKey, QVariant inValue);
	bool renameItem(QString inSrc, QString inDst);
	void removeItem(QString inKey);

protected:
	QQuickItem* createValueEditor(QQuickItem* inParent)override;
	QPropertyHandle* findOrCreateChildHandle(const QString& inSubName) override;
	Type type() override { return Type::Associative; };

private:
	QMetaAssociation mMetaAssociation;
};


#endif // QAssociativePropertyHandle_h__


#ifndef QAssociativePropertyHandle_h__
#define QAssociativePropertyHandle_h__

#include "QMetaContainer"
#include "IPropertyHandleImpl.h"

class QDETAILS_VIEW_API QPropertyHandleImpl_Associative: public IPropertyHandleImpl {
public:
	QPropertyHandleImpl_Associative(QPropertyHandle* inHandle);

	const QMetaAssociation& metaAssociation() const;

	void appendItem(QString inKey, QVariant inValue);
	bool renameItem(QString inSrc, QString inDst);
	void removeItem(QString inKey);

protected:
	Type type() override { return Type::Associative; };
	QQuickItem* createValueEditor(QQuickItem* inParent)override;

private:
	QMetaAssociation mMetaAssociation;
};


#endif // QAssociativePropertyHandle_h__


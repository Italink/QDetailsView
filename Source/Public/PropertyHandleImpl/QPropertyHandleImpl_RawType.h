#ifndef QPropertyHandleImpl_RawType_h__
#define QPropertyHandleImpl_RawType_h__

#include "IPropertyHandleImpl.h"

class QDETAILS_VIEW_API QPropertyHandleImpl_RawType : public IPropertyHandleImpl {
public:
	QPropertyHandleImpl_RawType(QPropertyHandle* inHandle);
protected:
	QQuickItem* createValueEditor(QQuickItem* inParent) override;
	Type type() override { return Type::RawType; };
};

#endif // QPropertyHandleImpl_RawType_h__

#ifndef PropertyTypeCustomization_ObjectDefault_h__
#define PropertyTypeCustomization_ObjectDefault_h__

#include "IPropertyTypeCustomization.h"

class PropertyTypeCustomization_ObjectDefault : public IPropertyTypeCustomization {
protected:
	virtual void customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder) override;
};

#endif // PropertyTypeCustomization_ObjectDefault_h__

#ifndef PropertyTypeCustomization_Matrix4x4_h__
#define PropertyTypeCustomization_Matrix4x4_h__

#include "IPropertyTypeCustomization.h"

class PropertyTypeCustomization_Matrix4x4 : public IPropertyTypeCustomization {
protected:
	virtual void customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder) override;
};

#endif // PropertyTypeCustomization_Matrix4x4_h__

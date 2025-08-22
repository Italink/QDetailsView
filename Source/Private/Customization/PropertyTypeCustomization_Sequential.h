#ifndef PropertyTypeCustomization_Sequential_h__
#define PropertyTypeCustomization_Sequential_h__

#include "IPropertyTypeCustomization.h"

class PropertyTypeCustomization_Sequential : public IPropertyTypeCustomization {
protected:
	virtual void customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder) override;
};

#endif // PropertyTypeCustomization_Sequential_h__

#ifndef PropertyTypeCustomization_Associative_h__
#define PropertyTypeCustomization_Associative_h__

#include "IPropertyTypeCustomization.h"

class PropertyTypeCustomization_Associative : public IPropertyTypeCustomization {
protected:
	virtual void customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder) override;
};

#endif // PropertyTypeCustomization_Associative_h__

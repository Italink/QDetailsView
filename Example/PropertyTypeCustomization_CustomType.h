#ifndef PropertyTypeCustomization_CustomType_h__
#define PropertyTypeCustomization_CustomType_h__

#include "IPropertyTypeCustomization.h"

class PropertyTypeCustomization_CustomType : public IPropertyTypeCustomization {
protected:
	virtual void customizeHeaderRow(QPropertyHandle* inPropertyHandle, QQuickDetailsViewRowBuilder* inBuilder) override;
	virtual void customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder) override;
};

#endif // PropertyTypeCustomization_CustomType_h__

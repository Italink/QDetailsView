#ifndef ClassLayoutCustomization_Default_h__
#define ClassLayoutCustomization_Default_h__

#include "IClassLayoutCustomization.h"

class ClassLayoutCustomization_Default : public IClassLayoutCustomization {
protected:
	virtual void customizeLayout(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder) override;
};

#endif // ClassLayoutCustomization_Default_h__

#ifndef IClassLayoutCustomization_h__
#define IClassLayoutCustomization_h__

#include <QSharedPointer>
#include "QDetailsViewAPI.h"
#include "QPropertyHandleImpl_Object.h"

class QQuickDetailsViewLayoutBuilder;

class QDETAILS_VIEW_API IClassLayoutCustomization :public QEnableSharedFromThis<IClassLayoutCustomization> {
public:
	virtual ~IClassLayoutCustomization() {}

	virtual void customizeLayout(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder) = 0;
};


#endif // IClassLayoutCustomization_h__

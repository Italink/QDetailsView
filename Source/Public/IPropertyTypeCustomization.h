#ifndef IPropertyTypeCustomization_h__
#define IPropertyTypeCustomization_h__

#include <QSharedPointer>
#include "QDetailsViewAPI.h"

class QPropertyHandle;
class QQuickDetailsViewRowBuilder;
class QQuickDetailsViewLayoutBuilder;

class QDETAILS_VIEW_API IPropertyTypeCustomization :public  QEnableSharedFromThis<IPropertyTypeCustomization>
{
public:
	virtual void customizeHeader(QPropertyHandle* inPropertyHandle, QQuickDetailsViewRowBuilder* inBuilder);
	virtual void customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder);
};

#endif // IPropertyTypeCustomization_h__

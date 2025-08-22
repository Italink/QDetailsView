#ifndef IDetailCustomization_h__
#define IDetailCustomization_h__

#include <QSharedPointer>
#include "QDetailsViewAPI.h"

class QQuickDetailsViewLayoutBuilder;

class QDETAILS_VIEW_API IDetailCustomization :public QEnableSharedFromThis<IDetailCustomization> {
public:
	virtual ~IDetailCustomization() {}

	virtual void customizeDetails(const QObject* inObject, QQuickDetailsViewLayoutBuilder* inBuilder) = 0;
};


#endif // IDetailCustomization_h__

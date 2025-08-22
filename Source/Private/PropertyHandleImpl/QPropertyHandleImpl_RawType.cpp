#include "PropertyHandleImpl/IPropertyHandleImpl.h"
#include "PropertyHandleImpl/QPropertyHandleImpl_RawType.h"
#include "QQuickDetailsViewMananger.h"

QPropertyHandleImpl_RawType::QPropertyHandleImpl_RawType(QPropertyHandle* inHandle)
	: IPropertyHandleImpl(inHandle)
{

}

QQuickItem* QPropertyHandleImpl_RawType::createValueEditor(QQuickItem* inParent)
{
	return QQuickDetailsViewManager::Get()->createValueEditor(mHandle, inParent);
}

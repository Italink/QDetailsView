#include "IPropertyTypeCustomization.h"
#include "QQuickDetailsViewLayoutBuilder.h"

void IPropertyTypeCustomization::customizeHeaderRow(QPropertyHandle* inPropertyHandle, QQuickDetailsViewRowBuilder* inBuilder)
{
	inBuilder->makePropertyRow(inPropertyHandle);
}

void IPropertyTypeCustomization::customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder)
{

}


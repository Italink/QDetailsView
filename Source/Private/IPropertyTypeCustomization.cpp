#include "IPropertyTypeCustomization.h"
#include "QQuickDetailsViewLayoutBuilder.h"

void IPropertyTypeCustomization::customizeHeader(QPropertyHandle* inPropertyHandle, QQuickDetailsViewHeaderRowBuilder* inBuilder)
{
	inBuilder->makePropertyHeader(inPropertyHandle);
}

void IPropertyTypeCustomization::customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder)
{

}


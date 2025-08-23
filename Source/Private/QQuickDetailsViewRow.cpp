#include "QQuickDetailsViewRow.h"
#include "qqmlcontext.h"
#include "qsequentialiterable.h"
#include <QAssociativeIterable>
#include "QQuickDetailsViewLayoutBuilder.h"
#include "QQuickDetailsViewMananger.h"

void IDetailsViewRow::addChild(QSharedPointer<IDetailsViewRow> inChild)
{
	mChildren << inChild;
	inChild->mParent = this;
}

void IDetailsViewRow::clear()
{
	mChildren.clear();
}

QDetailsViewRow_Property::QDetailsViewRow_Property(QPropertyHandle* inHandle)
{
	setHandle(inHandle);
}

void QDetailsViewRow_Property::setupItem(QQuickItem* inParent)
{
	QQuickDetailsViewHeaderRowBuilder builder(inParent);
    if (mPropertyTypeCustomization) {
        mPropertyTypeCustomization->customizeHeader(mHandle, &builder);
        return;
    }
	builder.makePropertyHeader(mHandle);
}

void QDetailsViewRow_Property::attachChildren()
{
	if (mPropertyTypeCustomization){
		QQuickDetailsViewLayoutBuilder builder(this);
		mPropertyTypeCustomization->customizeChildren(mHandle, &builder);
	}
}

void QDetailsViewRow_Property::setHandle(QPropertyHandle* inHandle)
{
	mHandle = inHandle;
	if (!inHandle)
		return;
	mPropertyTypeCustomization = QQuickDetailsViewManager::Get()->getCustomPropertyType(inHandle);
}

QDetailsViewRow_Custom::QDetailsViewRow_Custom(QQuickItem* inItem)
    : mRowItem(inItem)
{

}

void QDetailsViewRow_Custom::setupItem(QQuickItem* inParent)
{
    mRowItem->setParentItem(inParent);
}

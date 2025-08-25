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
	QQuickDetailsViewRowBuilder builder(this, inParent);
    if (mPropertyTypeCustomization) {
        mPropertyTypeCustomization->customizeHeader(mHandle, &builder);
        return;
    }
	builder.makePropertyRow(mHandle);
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

QDetailsViewRow_Custom::QDetailsViewRow_Custom(std::function<void(QQuickDetailsViewRowBuilder*)> inRowCreator)
	: mRowCreator(inRowCreator)
{

}

void QDetailsViewRow_Custom::setupItem(QQuickItem* inParent)
{
	if (mRowCreator) {
		QQuickDetailsViewRowBuilder builder(this, inParent);
		mRowCreator(&builder);
	}
}

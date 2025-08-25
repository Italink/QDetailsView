#include "QQuickDetailsViewRow.h"
#include "QQuickDetailsViewLayoutBuilder.h"
#include "QQuickDetailsViewMananger.h"
#include "QQuickDetailsViewModel.h"

void IDetailsViewRow::addChild(QSharedPointer<IDetailsViewRow> inChild)
{
	mChildren << inChild;
	inChild->mModel = mModel;
	inChild->mParent = this;
}

void IDetailsViewRow::clear()
{
	mChildren.clear();
}

QQuickDetailsViewModel* IDetailsViewRow::model()
{
	return mModel;
}

void IDetailsViewRow::invalidateChildren()
{
	mModel->beginReset();
	mChildren.clear();
	attachChildren();
	mModel->endReset();
}

QDetailsViewRow_Property::QDetailsViewRow_Property(QPropertyHandle* inHandle)
{
	setHandle(inHandle);
}

QDetailsViewRow_Property::~QDetailsViewRow_Property()
{
	if (mStructureChangedConnection) {
		QObject::disconnect(mStructureChangedConnection);
	}
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
	mStructureChangedConnection = QObject::connect(inHandle, &QPropertyHandle::asStructureChanged, [this]() {
		invalidateChildren();
	});
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

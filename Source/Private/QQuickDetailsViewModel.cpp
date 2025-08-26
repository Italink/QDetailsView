#include "QQuickDetailsViewModel.h"
#include "QQuickDetailsViewRow.h"
#include "QQuickDetailsViewLayoutBuilder.h"

QQuickDetailsViewModel::QQuickDetailsViewModel(QObject* parent)
	: QAbstractItemModel(parent) 
    , mRoot(new QDetailsViewRow_Property(nullptr))
{
    mRoot->mModel = this;
}

QVariant QQuickDetailsViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();
    IDetailsViewRow* node = static_cast<IDetailsViewRow*>(index.internalPointer());
    return node->name();
}

Qt::ItemFlags QQuickDetailsViewModel::flags(const QModelIndex& index) const {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex QQuickDetailsViewModel::index(int row, int column, const QModelIndex& parent) const {
	if (column < 0 || column >= columnCount(parent)) {
		return QModelIndex();
	}

	IDetailsViewRow* parentNode = nullptr;
	if (!parent.isValid()) {
		parentNode = mRoot.get(); 
	}
	else {
		parentNode = static_cast<IDetailsViewRow*>(parent.internalPointer());
		if (!parentNode) {
			return QModelIndex();
		}
	}

	if (row < 0 || row >= parentNode->mChildren.size()) {
		return QModelIndex(); 
	}

	IDetailsViewRow* childNode = parentNode->mChildren[row].get();
	QModelIndex childIndex = createIndex(row, column, childNode);
	childNode->setModelIndex(childIndex);
	return childIndex;
}

QModelIndex QQuickDetailsViewModel::parent(const QModelIndex& index) const {
	if (!index.isValid())
		return QModelIndex();

	IDetailsViewRow* node = static_cast<IDetailsViewRow*>(index.internalPointer());
	IDetailsViewRow* parentNode = node->mParent;

	if (!parentNode || parentNode == mRoot.get()) {
		return QModelIndex();
	}

	IDetailsViewRow* grandparent = parentNode->mParent;
	int parentRow = grandparent ? grandparent->mChildren.indexOf(parentNode) : -1;

	if (parentRow == -1) {
		return QModelIndex();
	}

	QModelIndex parentIndex = createIndex(parentRow, 0, parentNode);
	parentNode->setModelIndex(parentIndex);
	return parentIndex;
}

int QQuickDetailsViewModel::rowCount(const QModelIndex& parent) const {
    if (!parent.isValid())
        return mRoot->mChildren.size();
    IDetailsViewRow* node = static_cast<IDetailsViewRow*>(parent.internalPointer());
    return node->mChildren.count();
}

void QQuickDetailsViewModel::setObject(QObject* inObject)
{
    mObject = inObject;
    mRoot->setHandle(QPropertyHandle::FindOrCreate(mObject));
    mRoot->invalidateChildren();
}

QObject* QQuickDetailsViewModel::getObject() const
{
    return mObject;
}

QModelIndex QQuickDetailsViewModel::indexForRow(IDetailsViewRow* row) const
{
	if (!row || row == mRoot.get()) {
		return QModelIndex();
	}

	IDetailsViewRow* parentRow = row->mParent;
	if (!parentRow) {
		return QModelIndex();
	}

	int rowNum = -1;
	for (int i = 0; i < parentRow->mChildren.size(); ++i) {
		if (parentRow->mChildren[i].data() == row) {
			rowNum = i;
			break;
		}
	}
	if (rowNum == -1) { 
		return QModelIndex();
	}
	QModelIndex parentIndex = indexForRow(parentRow);
	return createIndex(rowNum, 0, row);
}

void QQuickDetailsViewModel::updateRowIndex(IDetailsViewRow* row)
{
	if (!row) return;

	QModelIndex oldIndex = row->modelIndex();
	QModelIndex newIndex = indexForRow(row);

	if (oldIndex != newIndex) {
		row->setModelIndex(newIndex);
		dataChanged(newIndex, newIndex);
	}
}

int QQuickDetailsViewModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

QHash<int, QByteArray> QQuickDetailsViewModel::roleNames() const {
    return {
        { Roles::name,"name" },
    };
}

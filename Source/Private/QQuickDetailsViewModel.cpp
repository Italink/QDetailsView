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
	if (column < 0 || column >= columnCount(parent)) { // 假设 columnCount 返回 1，需自行实现
		return QModelIndex();
	}

	// 2. 确定父节点：parent 无效 → 父节点是 mRoot；否则 → 父节点是 parent 索引指向的节点
	IDetailsViewRow* parentNode = nullptr;
	if (!parent.isValid()) {
		parentNode = mRoot.get(); // 根节点的子节点，父节点是 mRoot
	}
	else {
		// 从 parent 索引中获取其指向的节点
		parentNode = static_cast<IDetailsViewRow*>(parent.internalPointer());
		// 额外检查：parentNode 不能为空（防止非法指针）
		if (!parentNode) {
			return QModelIndex();
		}
	}

	// 3. 检查 row 是否在父节点的子节点范围内（核心越界检查）
	if (row < 0 || row >= parentNode->mChildren.size()) {
		return QModelIndex(); // row 越界，返回无效索引
	}

	// 4. 从父节点的子节点中获取目标节点，创建并返回索引
	IDetailsViewRow* childNode = parentNode->mChildren[row].get();
	return createIndex(row, column, childNode);
}

QModelIndex QQuickDetailsViewModel::parent(const QModelIndex& index) const {
    if (!index.isValid())
        return QModelIndex();
    IDetailsViewRow* node = static_cast<IDetailsViewRow*>(index.internalPointer());
    if (node->mParent) {
        return createIndex(mRoot->mChildren.indexOf(node->mParent), 0, node->mParent);
    }
    return QModelIndex();
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
    mRoot->setHandle(QPropertyHandle::FindOrCreate(mObject, ""));
    mRoot->invalidateChildren();
}

QObject* QQuickDetailsViewModel::getObject() const
{
    return mObject;
}

void QQuickDetailsViewModel::beginReset()
{
    beginResetModel();
}

void QQuickDetailsViewModel::endReset()
{
    endResetModel();
}

int QQuickDetailsViewModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

QHash<int, QByteArray> QQuickDetailsViewModel::roleNames() const {
    return {
        { Roles::name,"name" },
    };
}

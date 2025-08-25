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
	if (column < 0 || column >= columnCount(parent)) { // ���� columnCount ���� 1��������ʵ��
		return QModelIndex();
	}

	// 2. ȷ�����ڵ㣺parent ��Ч �� ���ڵ��� mRoot������ �� ���ڵ��� parent ����ָ��Ľڵ�
	IDetailsViewRow* parentNode = nullptr;
	if (!parent.isValid()) {
		parentNode = mRoot.get(); // ���ڵ���ӽڵ㣬���ڵ��� mRoot
	}
	else {
		// �� parent �����л�ȡ��ָ��Ľڵ�
		parentNode = static_cast<IDetailsViewRow*>(parent.internalPointer());
		// �����飺parentNode ����Ϊ�գ���ֹ�Ƿ�ָ�룩
		if (!parentNode) {
			return QModelIndex();
		}
	}

	// 3. ��� row �Ƿ��ڸ��ڵ���ӽڵ㷶Χ�ڣ�����Խ���飩
	if (row < 0 || row >= parentNode->mChildren.size()) {
		return QModelIndex(); // row Խ�磬������Ч����
	}

	// 4. �Ӹ��ڵ���ӽڵ��л�ȡĿ��ڵ㣬��������������
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

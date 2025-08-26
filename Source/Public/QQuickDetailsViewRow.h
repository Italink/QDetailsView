#ifndef QQuickDetailsViewRow_h__
#define QQuickDetailsViewRow_h__

#include <QObject>
#include <QMap>
#include <QAbstractItemModel>
#include <QMetaProperty>
#include <QQuickItem>
#include "QPropertyHandle.h"
#include "IPropertyTypeCustomization.h"
#include "QDetailsViewAPI.h"

class QDETAILS_VIEW_API IDetailsViewRow {
    friend class QQuickDetailsViewModel;
public:
	~IDetailsViewRow() {};

    void setName(QString inName);
    virtual QString name();

    virtual void setupItem(QQuickItem* inParent){}
    virtual void attachChildren() {}
    virtual void addChild(QSharedPointer<IDetailsViewRow> inChild);

    void clear();
    QQuickDetailsViewModel* model();
	QModelIndex modelIndex() const { return mModelIndex; }
	void setModelIndex(const QModelIndex& index) { mModelIndex = index; }
	int rowNumber() const {
		if (!mParent) return -1;
		return mParent->mChildren.indexOf(const_cast<IDetailsViewRow*>(this));
	}
    void invalidateChildren();
protected:
    QString mName;
    QQuickDetailsViewModel* mModel = nullptr;
    QModelIndex mModelIndex;
    IDetailsViewRow* mParent = nullptr;
    QList<QSharedPointer<IDetailsViewRow>> mChildren;
};

class QDETAILS_VIEW_API QDetailsViewRow_Property : public IDetailsViewRow {
public:
    QDetailsViewRow_Property(QPropertyHandle* inHandle);
    ~QDetailsViewRow_Property();
    void setHandle(QPropertyHandle* inHandle);
    void setupItem(QQuickItem* inParent) override;
    void attachChildren() override;
protected:
    QPropertyHandle* mHandle = nullptr;
    QMetaObject::Connection mStructureChangedConnection;
    QSharedPointer<IPropertyTypeCustomization> mPropertyTypeCustomization;
};

class QDETAILS_VIEW_API QDetailsViewRow_Custom : public IDetailsViewRow {
public:
    QDetailsViewRow_Custom(std::function<void(QQuickDetailsViewRowBuilder*)> inRowCreator);
protected:
	QString name() override { return "Custom"; }
	void setupItem(QQuickItem* inParent) override;
private:
    std::function<void(QQuickDetailsViewRowBuilder*)> mRowCreator;
};

#endif // QQuickDetailsViewRow_h__

#ifndef QQuickDetailsViewModel_h__
#define QQuickDetailsViewModel_h__

#include <QObject>
#include <QMap>
#include <QAbstractItemModel>
#include <QMetaProperty>
#include <QQuickItem>
#include "QDetailsViewAPI.h"

class IDetailsViewRow;
class QDetailsViewRow_Property;

class QDETAILS_VIEW_API QQuickDetailsViewModel : public QAbstractItemModel {
    Q_OBJECT
    enum Roles {
        name = 0,
    };
    friend class IDetailsViewRow;
public:
    QQuickDetailsViewModel(QObject* parent = 0);
    ~QQuickDetailsViewModel() {}
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setObject(QObject* inObject);
    QObject* getObject() const;

	QModelIndex indexForRow(IDetailsViewRow* row) const;
	void updateRowIndex(IDetailsViewRow* row);

private:
    QSharedPointer<QDetailsViewRow_Property> mRoot;
    QObject* mObject;
};

#endif // QQuickDetailsViewModel_h__

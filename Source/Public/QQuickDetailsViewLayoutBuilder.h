#ifndef QQuickDetailsViewLayoutBuilder_h__
#define QQuickDetailsViewLayoutBuilder_h__

#include "QQuickDetailsViewRow.h"

class QDETAILS_VIEW_API QQuickDetailsViewRowBuilder {
public:
	QQuickDetailsViewRowBuilder(IDetailsViewRow* inRow, QQuickItem* inRootItem);
	QPair<QQuickItem*, QQuickItem*> makeNameValueSlot();

	IDetailsViewRow* row() const;
	QQuickItem* rootItem() const;

	void makePropertyRow(QPropertyHandle* inHandle);
	QQuickItem* setupItem(QQuickItem* inParent, QString inQmlCode);
	void setupLabel(QQuickItem* inParent, QString inText);
	void setHeightProxy(QQuickItem* inProxyItem);
private:
	IDetailsViewRow* mRow = nullptr;
	QQuickItem* mRootItem = nullptr;
};

class  QDETAILS_VIEW_API QQuickDetailsViewLayoutBuilder {
public:
	QQuickDetailsViewLayoutBuilder(IDetailsViewRow* inRootRow);

	IDetailsViewRow* row() const;

	void addCustomRow(std::function<void(QQuickDetailsViewRowBuilder*)> inCustomRowCreator, QString inOverrideName = "");
	void addProperty(QPropertyHandle* inPropertyHandle, QString inOverrideName = "");
	void addObject(QObject* inObject);
private:
	IDetailsViewRow* mRootRow = nullptr;
};

#endif // QQuickDetailsViewLayoutBuilder_h__

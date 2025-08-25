#ifndef QQuickDetailsViewLayoutBuilder_h__
#define QQuickDetailsViewLayoutBuilder_h__

#include "QQuickDetailsViewRow.h"

class QDETAILS_VIEW_API QQuickDetailsViewRowBuilder {
public:
	QQuickDetailsViewRowBuilder(IDetailsViewRow* inRow, QQuickItem* inRootItem);
	QPair<QQuickItem*, QQuickItem*> makeNameValueSlot();
	void makePropertyRow(QPropertyHandle* inHandle);
	QQuickItem* rootItem();
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
	void addCustomRow(std::function<void(QQuickDetailsViewRowBuilder*)> inCustomRowCreator);
	void addProperty(QPropertyHandle* inPropertyHandle);
	void addObject(QObject* inObject);
private:
	IDetailsViewRow* mRootRow = nullptr;
};

#endif // QQuickDetailsViewLayoutBuilder_h__

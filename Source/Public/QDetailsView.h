#ifndef QDetailsView_h__
#define QDetailsView_h__

#include <QWidget>
#include <QQuickWidget>
#include "QQuickDetailsView.h"

class QDETAILS_VIEW_API QDetailsView : public QWidget {
	Q_OBJECT
public:
	explicit QDetailsView(QWidget* parent = nullptr);
	QQuickDetailsView* getQuickDetailsView() const;
	void setObject(QObject* inObject);
	QObject* getObject() const;
private:
	QQuickWidget* mQuickWidget;
	QQuickDetailsView* mQuickDetailsView;
};

#endif // QDetailsView_h__

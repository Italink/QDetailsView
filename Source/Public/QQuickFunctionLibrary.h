#ifndef QQuickFunctionLibrary_h__
#define QQuickFunctionLibrary_h__

#include <QObject>
#include <QVariant>
#include <QQuickItem>
#include "QDetailsViewAPI.h"

class QDETAILS_VIEW_API QQuickFunctionLibrary : public QObject {
	Q_OBJECT
public:
	static QQuickFunctionLibrary* Get();

	Q_INVOKABLE QString numberToString(QVariant var,int precision);
	Q_INVOKABLE void setCursorPos(qreal x, qreal y);
	Q_INVOKABLE void setOverrideCursorShape(Qt::CursorShape shape);
	Q_INVOKABLE void restoreOverrideCursorShape();
	Q_INVOKABLE void setCursorPosTest(QQuickItem* item, qreal x, qreal y);
};

class QDETAILS_VIEW_API QQuickLambdaHelper : public QObject {
	Q_OBJECT
		std::function<void()> mCallback;
public:
	QQuickLambdaHelper(std::function<void()>&& callback, QObject* parent)
		: QObject(parent)
		, mCallback(std::move(callback)) {}

	Q_SLOT void call() { mCallback(); }

	static QMetaObject::Connection connect(QObject* sender, const char* signal, std::function<void()>&& callback){
		if (!sender) 
			return {};
		return QObject::connect(sender, signal, new QQuickLambdaHelper(std::move(callback), sender), SLOT(call()));
	}
};

#endif // QQuickFunctionLibrary_h__

#ifndef QQuickFunctionLibrary_h__
#define QQuickFunctionLibrary_h__

#include <QObject>
#include <QVariant>
#include <QQuickItem>
#include "QDetailsViewAPI.h"

class QQuickLambdaHolder;
class QQuickLambdaHolder_OneParam;

class QDETAILS_VIEW_API QQuickFunctionLibrary : public QObject {
	Q_OBJECT
public:
	static QQuickFunctionLibrary* Get();

	Q_INVOKABLE QString numberToString(QVariant var,int precision);
	Q_INVOKABLE void setCursorPos(qreal x, qreal y);
	Q_INVOKABLE void setOverrideCursorShape(Qt::CursorShape shape);
	Q_INVOKABLE void restoreOverrideCursorShape();
	Q_INVOKABLE void setCursorPosTest(QQuickItem* item, qreal x, qreal y);


	static QMetaObject::Connection connect(QObject* sender, const char* signal, QObject* receiver, std::function<void()> callback);
	static QMetaObject::Connection connect(QObject* sender, const char* signal, QObject* receiver, std::function<void(QVariant)> callback);
	static QMetaObject::Connection connect(QObject* sender, const char* signal, QObject* receiver, std::function<void(QVariant, QVariant)> callback);
};

class QDETAILS_VIEW_API QQuickLambdaHolder : public QObject {
	Q_OBJECT
	std::function<void()> mCallback;
public:
	QQuickLambdaHolder(std::function<void()> callback, QObject* parent)
		: QObject(parent)
		, mCallback(std::move(callback)) {}

	Q_SLOT void call() { mCallback(); }
};

class QDETAILS_VIEW_API QQuickLambdaHolder_OneParam : public QObject {
	Q_OBJECT
		std::function<void(QVariant)> mCallback;
public:
	QQuickLambdaHolder_OneParam(std::function<void(QVariant)> callback, QObject* parent)
		: QObject(parent)
		, mCallback(std::move(callback)) {
	}

	Q_SLOT void call(QVariant inParam0) { mCallback(inParam0); }
};

class QDETAILS_VIEW_API QQuickLambdaHolder_TwoParams : public QObject {
	Q_OBJECT
	std::function<void(QVariant, QVariant)> mCallback;
public:
	QQuickLambdaHolder_TwoParams(std::function<void(QVariant, QVariant)> callback, QObject* parent)
		: QObject(parent)
		, mCallback(std::move(callback)) {
	}

	Q_SLOT void call(QVariant inParam0, QVariant inParam1) { mCallback(inParam0, inParam1); }
};

#endif // QQuickFunctionLibrary_h__

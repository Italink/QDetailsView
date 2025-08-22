#include "QQuickFunctionLibrary.h"
#include <QCursor>

QQuickFunctionLibrary* QQuickFunctionLibrary::Get()
{
	static QQuickFunctionLibrary Ins;
	return &Ins;
}

QString QQuickFunctionLibrary::numberToString(QVariant var, int precision)
{
	double value = var.toDouble();
	return QString::number(value, 'f', precision);
}

void QQuickFunctionLibrary::setCursorPos(qreal x, qreal y)
{
	//qDebug() << QCursor::pos();
	//QCursor::setPos(x, y);
}

void QQuickFunctionLibrary::setCursorPosTest(QQuickItem* item, qreal x, qreal y)
{
	QPointF global = item->mapToGlobal(x, y);
	QCursor::setPos(global.x(), global.y());
	qDebug() << x << y << global << QCursor::pos();
}

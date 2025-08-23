#include "QQuickFunctionLibrary.h"
#include <QCursor>
#include <QApplication>

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
	QCursor::setPos(x, y);
}

void QQuickFunctionLibrary::setOverrideCursorShape(Qt::CursorShape shape)
{
	qApp->setOverrideCursor(shape);
}

void QQuickFunctionLibrary::restoreOverrideCursorShape()
{
	qApp->restoreOverrideCursor();
}

void QQuickFunctionLibrary::setCursorPosTest(QQuickItem* item, qreal x, qreal y)
{
	QPointF global = item->mapToGlobal(x, y);
	QCursor::setPos(global.x(), global.y());
	qDebug() << x << y << global << QCursor::pos();
}

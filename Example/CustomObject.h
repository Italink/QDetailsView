#ifndef CustomObject_h__
#define CustomObject_h__

#include <QObject>
#include <qvectornd.h>
#include <QFile>
#include <QDir>
#include <QColor>
#include <QMatrix4x4>
#include "CommonInclude.h"
#include "CustomGadget.h"
#include "CustomType.h"

class QCustomObject :public QObject {
	Q_OBJECT
		Q_CLASSINFO("LimitedDouble", "Min=0,Max=10")
public:
	enum QCustomEnum {
		One,
		Two,
		Three
	};
	Q_ENUM(QCustomEnum);

	Q_PROPERTY_VAR(int, Int) = 0;
	Q_PROPERTY_VAR(float, Float) = 1.23f;
	Q_PROPERTY_VAR(double, LimitedDouble) = 5;
	Q_PROPERTY_VAR(QString, String);
	Q_PROPERTY_VAR(QDir, Directory) = QDir("D:/");
	Q_PROPERTY_VAR(QVector2D, Vec2) = QVector2D(1, 2);
	Q_PROPERTY_VAR(QVector3D, Vec3) = QVector3D(1, 2, 3);
	Q_PROPERTY_VAR(QVector4D, Vec4) = QVector4D(1, 2, 3, 4);
	Q_PROPERTY_VAR(QMatrix4x4, Mat4);
	Q_PROPERTY_VAR(QColor, Color);
	Q_PROPERTY_VAR(QList<QColor>, ColorList) = { Qt::red,Qt::green,Qt::blue };

	typedef QMap<QString, QColor> StringColorMap;
	Q_PROPERTY_VAR(StringColorMap, ColorMap) = { {"Red",Qt::red},{"Green",Qt::green},{"Blue",Qt::blue} };

	Q_PROPERTY_VAR(QCustomEnum, CustomEnum) = QCustomEnum::One;
	Q_PROPERTY_VAR(QCustomType, CustomType);
	Q_PROPERTY_VAR(QCutsomGadget, CustomGadget);
	Q_PROPERTY_VAR(QCutsomGadget*, CustomGadgetPtr) = new QCutsomGadget;
	Q_PROPERTY_VAR(QSharedPointer<QCutsomGadget>, CustomGadgetSharedPtr) = QSharedPointer<QCutsomGadget>::create();
	Q_PROPERTY_VAR(QCustomObject*, SubCustomObject) = nullptr;
};

#endif // CustomObject_h__
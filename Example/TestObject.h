#ifndef TestObject_h__
#define TestObject_h__

#include <QObject>
#include <qvectornd.h>
#include <QFile>
#include <QDir>
#include <QColor>
#include <QMatrix4x4>

#define Q_PROPERTY_VAR(Type,Name)\
    Q_PROPERTY(Type Name READ get_##Name WRITE set_##Name) \
    Type get_##Name(){ return Name; } \
    void set_##Name(Type var){ \
        Name = var;  \
		qDebug()<<"Set "<<#Name<<": "<<var; \
    } \
    Type Name


class TestInlineGadget {
	Q_GADGET
		//Q_META_BEGIN(TestInlineGadget)
		//Q_META_P_NUMBER_LIMITED(LimitedDouble, 0, 100)
		//Q_META_END()
public:
	TestInlineGadget() { qDebug() << "Create InlineGadge"; }
	~TestInlineGadget() { qDebug() << "Release InlineGadge"; }
	Q_PROPERTY_VAR(double, LimitedDouble) = 1;
	Q_PROPERTY_VAR(QString, Desc) = "This is inline Gadget";
};

class TestInlineObject : public QObject {
	Q_OBJECT
public:
	Q_PROPERTY_VAR(QString, Desc) = "This is inline Object";
};

static QDebug operator<<(QDebug debug, const std::string& str) {
	return debug << QString::fromStdString(str);
}

static QDebug operator<<(QDebug debug, const TestInlineGadget& gadget) {
	return debug << gadget.LimitedDouble << gadget.Desc;
}

static QDebug operator<<(QDebug debug, const std::shared_ptr<TestInlineGadget>& gadget) {
	return debug << gadget->LimitedDouble << gadget->Desc;
}

//static QDebug operator<<(QDebug debug, const TestInlineObject* object) {
//	return debug << object->Desc;
//}


class TestObject :public QObject {
	Q_OBJECT

	Q_CLASSINFO("LimitedDouble", "Min=0,Max=10")
public:
	enum TestEnum {
		One,
		Two,
		Three
	};
	Q_ENUM(TestEnum);

	Q_PROPERTY_VAR(int, Int) = 0;
	Q_PROPERTY_VAR(float, Float) = 1.23f;
	Q_PROPERTY_VAR(double, LimitedDouble) = 5;
	Q_PROPERTY_VAR(TestEnum, Enum) = TestEnum::One;
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

	Q_PROPERTY_VAR(TestInlineGadget*, InlineGadgetPtr) = new TestInlineGadget;
	Q_PROPERTY_VAR(TestInlineObject*, InlineObject) = new TestInlineObject;

};

Q_DECLARE_METATYPE(QSharedPointer<TestInlineGadget>);
Q_DECLARE_METATYPE(std::shared_ptr<TestInlineGadget>);

#endif // TestObject_h__
#include "QQuickDetailsViewMananger.h"
#include "QPropertyHandle.h"
#include "QQuickFunctionLibrary.h"
#include <QRegularExpression>
#include <QDir>
#include <QMetaType>

#define REGINTER_NUMBER_EDITOR_CREATOR(TypeName, DefaultPrecision) \
	registerTypeEditor(QMetaType::fromType<TypeName>(), [](QPropertyHandle* handle, QQuickItem* parent)->QQuickItem* { \
		QQmlEngine* engine = qmlEngine(parent);\
		QQmlContext* context = qmlContext(parent);\
		QQmlComponent nameComp(engine);\
		nameComp.setData(R"(
					import QtQuick;
					import QtQuick.Controls;
					import "qrc:/Resources/Qml/ValueEditor"
					NumberBox{
						anchors.verticalCenter: parent.verticalCenter
						width: parent.width
					}
				)", QUrl());\
		QVariantMap initialProperties; \
		initialProperties["parent"] = QVariant::fromValue(parent);\
		auto valueEditor = qobject_cast<QQuickItem*>(nameComp.createWithInitialProperties(initialProperties, context));\
		if (!nameComp.errors().isEmpty()) { \
			qDebug() << nameComp.errorString(); \
		} \
		valueEditor->setParentItem(parent);\
		TypeName min = handle->getMetaData("Min").toDouble();\
		TypeName max = handle->getMetaData("Max").toDouble();\
		double step = handle->getMetaData("Step").toDouble();\
		int precision = handle->getMetaData("Precision").toInt(); \
		valueEditor->setProperty("step", step <= 0.0001 ? 1 / pow(10, DefaultPrecision): step);\
		valueEditor->setProperty("number", handle->getVar());\
		valueEditor->setProperty("precision", precision == 0 ? DefaultPrecision:precision); \
		if (min < max) {\
			valueEditor->setProperty("min", min);\
			valueEditor->setProperty("max", max);\
			valueEditor->setProperty("isLimited", true);\
		}\
		connect(valueEditor, SIGNAL(valueChanged(QVariant)), handle, SLOT(setVar(QVariant))); \
		connect(handle, SIGNAL(asRequestRollback(QVariant)), valueEditor, SLOT(setNumber(QVariant))); \
		return valueEditor;\
	});


void QQuickDetailsViewManager::RegisterBasicTypeEditor() {
	REGINTER_NUMBER_EDITOR_CREATOR(int, 0);
	REGINTER_NUMBER_EDITOR_CREATOR(unsigned int, 0);
	REGINTER_NUMBER_EDITOR_CREATOR(size_t, 0);
	REGINTER_NUMBER_EDITOR_CREATOR(float, 2);
	REGINTER_NUMBER_EDITOR_CREATOR(double, 3);

	registerTypeEditor(QMetaType::fromType<QString>(), [](QPropertyHandle* handle, QQuickItem* parent)->QQuickItem* {
		QQmlEngine* engine = qmlEngine(parent);
		QQmlContext* context = qmlContext(parent);
		QQmlComponent comp(engine);
		comp.setData(R"(
					import QtQuick;
					import QtQuick.Controls;
					import "qrc:/Resources/Qml/ValueEditor"
					MultiLineTextBox{
					     anchors.verticalCenter: parent.verticalCenter
						 width: parent.width
					}
				)", QUrl());
		QVariantMap initialProperties;
		initialProperties["parent"] = QVariant::fromValue(parent);
		auto valueEditor = qobject_cast<QQuickItem*>(comp.createWithInitialProperties(initialProperties, context));
		if (!comp.errors().isEmpty()) {
			qDebug() << comp.errorString();
		}
		valueEditor->setParentItem(parent);
		valueEditor->setProperty("value", handle->getVar());
		connect(valueEditor, SIGNAL(asValueChanged(QVariant)), handle, SLOT(setVar(QVariant)));

		connect(handle, SIGNAL(asRequestRollback(QVariant)), valueEditor, SLOT(setValue(QVariant)));
		return valueEditor;
	});

	registerTypeEditor(QMetaType::fromType<QVector4D>(), [](QPropertyHandle* handle, QQuickItem* parent)->QQuickItem* {
		QQmlEngine* engine = qmlEngine(parent);
		QQmlContext* context = qmlContext(parent);
		QQmlComponent comp(engine);
		comp.setData(R"(
					import QtQuick;
					import QtQuick.Controls;
					import "qrc:/Resources/Qml/ValueEditor"
					Vec4Box{
						anchors.verticalCenter: parent.verticalCenter
						width: parent.width
					}
				)", QUrl());
		QVariantMap initialProperties;
		initialProperties["parent"] = QVariant::fromValue(parent);
		auto valueEditor = qobject_cast<QQuickItem*>(comp.createWithInitialProperties(initialProperties, context));
		if (!comp.errors().isEmpty()) {
			qDebug() << comp.errorString();
		}
		valueEditor->setParentItem(parent);
		valueEditor->setProperty("value", handle->getVar());
		connect(valueEditor, SIGNAL(asValueChanged(QVariant)), handle, SLOT(setVar(QVariant)));
		connect(handle, SIGNAL(asRequestRollback(QVariant)), valueEditor, SLOT(setValue(QVariant)));
		return valueEditor;
	});

	registerTypeEditor(QMetaType::fromType<QVector3D>(), [](QPropertyHandle* handle, QQuickItem* parent)->QQuickItem* {
		QQmlEngine* engine = qmlEngine(parent);
		QQmlContext* context = qmlContext(parent);
		QQmlComponent comp(engine);
		comp.setData(R"(
					import QtQuick;
					import QtQuick.Controls;
					import "qrc:/Resources/Qml/ValueEditor"
					Vec3Box{
						 anchors.verticalCenter: parent.verticalCenter
						 width: parent.width
					}
				)", QUrl());
		QVariantMap initialProperties;
		initialProperties["parent"] = QVariant::fromValue(parent);
		auto valueEditor = qobject_cast<QQuickItem*>(comp.createWithInitialProperties(initialProperties, context));
		if (!comp.errors().isEmpty()) {
			qDebug() << comp.errorString();
		}
		valueEditor->setParentItem(parent);
		valueEditor->setProperty("value", handle->getVar());
		connect(valueEditor, SIGNAL(asValueChanged(QVariant)), handle, SLOT(setVar(QVariant)));
		connect(handle, SIGNAL(asRequestRollback(QVariant)), valueEditor, SLOT(setValue(QVariant)));
		return valueEditor;
	});

	registerTypeEditor(QMetaType::fromType<QVector2D>(), [](QPropertyHandle* handle, QQuickItem* parent)->QQuickItem* {
		QQmlEngine* engine = qmlEngine(parent);
		QQmlContext* context = qmlContext(parent);
		QQmlComponent comp(engine);
		comp.setData(R"(
					import QtQuick;
					import QtQuick.Controls;
					import "qrc:/Resources/Qml/ValueEditor"
					Vec2Box{
						 anchors.verticalCenter: parent.verticalCenter
						 width: parent.width
					}
				)", QUrl());
		QVariantMap initialProperties;
		initialProperties["parent"] = QVariant::fromValue(parent);
		auto valueEditor = qobject_cast<QQuickItem*>(comp.createWithInitialProperties(initialProperties, context));
		if (!comp.errors().isEmpty()) {
			qDebug() << comp.errorString();
		}
		valueEditor->setParentItem(parent);
		valueEditor->setProperty("value", handle->getVar());
		connect(valueEditor, SIGNAL(asValueChanged(QVariant)), handle, SLOT(setVar(QVariant)));
		connect(handle, SIGNAL(asRequestRollback(QVariant)), valueEditor, SLOT(setValue(QVariant)));
		return valueEditor;
	});

	registerTypeEditor(QMetaType::fromType<QColor>(), [](QPropertyHandle* handle, QQuickItem* parent)->QQuickItem* {
		QQmlEngine* engine = qmlEngine(parent);
		QQmlContext* context = qmlContext(parent);
		QQmlComponent comp(engine);
		comp.setData(R"(
					import QtQuick;
					import QtQuick.Controls;
					import "qrc:/Resources/Qml/ValueEditor"
					ColorBox{
						 anchors.verticalCenter: parent.verticalCenter
						 width: parent.width
					}
				)", QUrl());
		QVariantMap initialProperties;
		initialProperties["parent"] = QVariant::fromValue(parent);
		auto valueEditor = qobject_cast<QQuickItem*>(comp.createWithInitialProperties(initialProperties, context));
		if (!comp.errors().isEmpty()) {
			qDebug() << comp.errorString();
		}
		valueEditor->setParentItem(parent);
		valueEditor->setProperty("value", handle->getVar());
		connect(valueEditor, SIGNAL(asValueChanged(QVariant)), handle, SLOT(setVar(QVariant)));
		connect(handle, SIGNAL(asRequestRollback(QVariant)), valueEditor, SLOT(setValue(QVariant)));
		return valueEditor;
	});

	QMetaType::registerConverterFunction(
		[](const void* src, void* target) -> bool {
			const QDir& dir = *static_cast<const QDir*>(src);
			QString& str = *static_cast<QString*>(target);
			str = dir.absolutePath();
			return true;
		},
		QMetaType::fromType<QDir>(),
		QMetaType::fromType<QString>()
	);

	QMetaType::registerConverterFunction(
		[](const void* src, void* target) -> bool {
			const QString& str = *static_cast<const QString*>(src);
			QDir& dir = *static_cast<QDir*>(target);
			dir = QDir(str);
			return true;
		},
		QMetaType::fromType<QString>(),
		QMetaType::fromType<QDir>()
	);

	registerTypeEditor(QMetaType::fromType<QDir>(), [](QPropertyHandle* handle, QQuickItem* parent)->QQuickItem* {
		QQmlEngine* engine = qmlEngine(parent);
		QQmlContext* context = qmlContext(parent);
		QQmlComponent comp(engine);
		comp.setData(R"(
					import QtQuick;
					import QtQuick.Controls;
					import "qrc:/Resources/Qml/ValueEditor"
					DirectorySelector{
						 anchors.verticalCenter: parent.verticalCenter
						 width: parent.width
					}
				)", QUrl());
		QVariantMap initialProperties;
		initialProperties["parent"] = QVariant::fromValue(parent);
		auto valueEditor = qobject_cast<QQuickItem*>(comp.createWithInitialProperties(initialProperties, context));
		if (!comp.errors().isEmpty()) {
			qDebug() << comp.errorString();
		}
		valueEditor->setParentItem(parent);
		valueEditor->setProperty("value", handle->getVar());
		connect(valueEditor, SIGNAL(asValueChanged(QVariant)), handle, SLOT(setVar(QVariant)));
		connect(handle, SIGNAL(asRequestRollback(QVariant)), valueEditor, SLOT(setValue(QVariant)));
		return valueEditor;
	});
}

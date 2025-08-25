#include "PropertyTypeCustomization_Matrix4x4.h"
#include "QQuickDetailsViewLayoutBuilder.h"
#include "QPropertyHandle.h"
#include <QMetaType>
#include <QObject>
#include <QMatrix4x4>
#include "QQuickFunctionLibrary.h"

void PropertyTypeCustomization_Matrix4x4::customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder)
{
	//inBuilder->addCustomRow([inPropertyHandle](QQuickDetailsViewRowBuilder* builder) {
	//	auto editorSlot = builder->makeNameValueSlot();
	//	builder->setupLabel(editorSlot.first, "0");
	//	auto valueItem = builder->setupItem(editorSlot.second, R"(
	//				import QtQuick;
	//				import QtQuick.Controls;
	//				import "qrc:/Resources/Qml/ValueEditor"
	//				Vec4Box{
	//					anchors.verticalCenter: parent.verticalCenter
	//					width: parent.width
	//				}
	//			)");
	//	builder->setHeightProxy(valueItem);
	//	QMatrix4x4 mat = inPropertyHandle->getVar().value<QMatrix4x4>();
	//	valueItem->setProperty("value", mat.row(0));

	//	QQuickFunctionLibrary::connect(valueItem, SIGNAL(asValueChanged(QVariant)), inPropertyHandle, [inPropertyHandle](QVariant var) {
	//		QMatrix4x4 mat = inPropertyHandle->getVar().value<QMatrix4x4>();
	//		mat.setRow(0, var.value<QVector4D>());
	//		inPropertyHandle->setVar(mat);
	//	});
	//	QQuickFunctionLibrary::connect(inPropertyHandle, SIGNAL(asRequestRollback(QVariant)), valueItem, [inPropertyHandle, valueItem](QVariant var) {
	//		QMatrix4x4 mat = var.value<QMatrix4x4>();
	//		valueItem->setProperty("value", mat.row(0));
	//	});
	//});

	inBuilder->addProperty(QPropertyHandle::FindOrCreate(
		inPropertyHandle->parent(),
		QMetaType::fromType<QVector4D>(),
		inPropertyHandle->createSubPath("Row 0"),
		[inPropertyHandle]() {
			return inPropertyHandle->getVar().value<QMatrix4x4>().row(0);
		},
		[inPropertyHandle](QVariant var) {
			QMatrix4x4 mat = inPropertyHandle->getVar().value<QMatrix4x4>();
			mat.setRow(0, var.value<QVector4D>());
			inPropertyHandle->setVar(mat);
		}
	));

	inBuilder->addProperty(QPropertyHandle::FindOrCreate(
		inPropertyHandle->parent(),
		QMetaType::fromType<QVector4D>(),
		inPropertyHandle->createSubPath("Row 1"),
		[inPropertyHandle]() {
			return inPropertyHandle->getVar().value<QMatrix4x4>().row(1);
		},
		[inPropertyHandle](QVariant var) {
			QMatrix4x4 mat = inPropertyHandle->getVar().value<QMatrix4x4>();
			mat.setRow(1, var.value<QVector4D>());
			inPropertyHandle->setVar(mat);
		}
	));

	inBuilder->addProperty(QPropertyHandle::FindOrCreate(
		inPropertyHandle->parent(),
		QMetaType::fromType<QVector4D>(),
		inPropertyHandle->createSubPath("Row 2"),
		[inPropertyHandle]() {
			return inPropertyHandle->getVar().value<QMatrix4x4>().row(2);
		},
		[inPropertyHandle](QVariant var) {
			QMatrix4x4 mat = inPropertyHandle->getVar().value<QMatrix4x4>();
			mat.setRow(2, var.value<QVector4D>());
			inPropertyHandle->setVar(mat);
		}
	));

	inBuilder->addProperty(QPropertyHandle::FindOrCreate(
		inPropertyHandle->parent(),
		QMetaType::fromType<QVector4D>(),
		inPropertyHandle->createSubPath("Row 3"),
		[inPropertyHandle]() {
			return inPropertyHandle->getVar().value<QMatrix4x4>().row(3);
		},
		[inPropertyHandle](QVariant var) {
			QMatrix4x4 mat = inPropertyHandle->getVar().value<QMatrix4x4>();
			mat.setRow(3, var.value<QVector4D>());
			inPropertyHandle->setVar(mat);
		}
	));
}


#include "PropertyTypeCustomization_CustomType.h"
#include "QQuickDetailsViewLayoutBuilder.h"
#include "QPropertyHandle.h"
#include <QMetaType>
#include <QObject>
#include <QRandomGenerator>
#include "QQuickDetailsViewModel.h"
#include "CustomType.h"
#include "QQuickFunctionLibrary.h"

void PropertyTypeCustomization_CustomType::customizeHeaderRow(QPropertyHandle* inPropertyHandle, QQuickDetailsViewRowBuilder* inBuilder)
{
	auto editorSlot = inBuilder->makeNameValueSlot();
	inPropertyHandle->createNameEditor(editorSlot.first);
	auto buttonItem = inBuilder->setupItem(editorSlot.second, R"(
					import QtQuick;
					import QtQuick.Controls;
					Button{
						anchors.horizontalCenter: parent.horizontalCenter
						anchors.verticalCenter: parent.verticalCenter
						width: 80	
						height: 20
						text: "Sort"
					}
				)");


	QQuickFunctionLibrary::connect(buttonItem, SIGNAL(clicked()), inPropertyHandle, [inPropertyHandle]() {
		QCustomType customType = inPropertyHandle->getVar().value<QCustomType>();
		std::sort(customType.Array.begin(), customType.Array.end());
		inPropertyHandle->setVar(QVariant::fromValue(customType));
	});
}

void PropertyTypeCustomization_CustomType::customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder)
{
	auto arrayHandle = QPropertyHandle::FindOrCreate(
		inPropertyHandle->parent(),
		QMetaType::fromType<QVector<int>>(),
		inPropertyHandle->createSubPath("Array"),
		[inPropertyHandle]() {
			return QVariant::fromValue(inPropertyHandle->getVar().value<QCustomType>().Array);
		},
		[inPropertyHandle](QVariant var) {
			QCustomType customType = inPropertyHandle->getVar().value<QCustomType>();
			customType.Array = var.value<QVector<int>>();
			inPropertyHandle->setVar(QVariant::fromValue(customType));
		}
	);

	auto arraySizeHandle = QPropertyHandle::FindOrCreate(
		inPropertyHandle->parent(),
		QMetaType::fromType<unsigned int>(),
		inPropertyHandle->createSubPath("ArraySize"),
		[inPropertyHandle]() {
			return inPropertyHandle->getVar().value<QCustomType>().ArraySize;
		},
		[inPropertyHandle, arrayHandle](QVariant var) {
			QCustomType customType = inPropertyHandle->getVar().value<QCustomType>();
			customType.ArraySize = var.toUInt();
			customType.Array.resize(customType.ArraySize);
			for (int i = 0; i < customType.ArraySize; ++i) {
				customType.Array[i] = QRandomGenerator::global()->bounded(-100000, 100000);
			}
			inPropertyHandle->setVar(QVariant::fromValue(customType));
			arrayHandle->invalidateStructure();
		}
	);


	inBuilder->addProperty(arraySizeHandle);
	inBuilder->addProperty(arrayHandle);
}


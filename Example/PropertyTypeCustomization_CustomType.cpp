#include "PropertyTypeCustomization_CustomType.h"
#include "QQuickDetailsViewLayoutBuilder.h"
#include "QPropertyHandle.h"
#include <QMetaType>
#include <QObject>
#include <QRandomGenerator>
#include "QQuickDetailsViewModel.h"
#include "CustomType.h"

void PropertyTypeCustomization_CustomType::customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder)
{
	auto model = inBuilder->row()->model();
	inBuilder->addProperty(QPropertyHandle::FindOrCreate(
		inPropertyHandle->parent(),
		QMetaType::fromType<unsigned int>(),
		inPropertyHandle->createSubPath("ArraySize"),
		[inPropertyHandle]() {
			return inPropertyHandle->getVar().value<QCustomType>().ArraySize;
		},
		[inPropertyHandle, model](QVariant var) {
			QCustomType customType = inPropertyHandle->getVar().value<QCustomType>();
			customType.ArraySize = var.toUInt();
			customType.Array.resize(customType.ArraySize);
			for (int i = 0; i < customType.ArraySize; ++i) {
				customType.Array[i] = QRandomGenerator::global()->bounded(-100000, 100000);
			}
			inPropertyHandle->setVar(QVariant::fromValue(customType));
			inPropertyHandle->invalidateStructure();
		}
	));

	inBuilder->addProperty(QPropertyHandle::FindOrCreate(
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
	));
}


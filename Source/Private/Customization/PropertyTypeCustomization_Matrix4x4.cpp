#include "PropertyTypeCustomization_Matrix4x4.h"
#include "QQuickDetailsViewLayoutBuilder.h"
#include "QPropertyHandle.h"
#include <QMetaType>

void PropertyTypeCustomization_Matrix4x4::customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailsViewLayoutBuilder* inBuilder)
{
	QPropertyHandle* position = QPropertyHandle::FindOrCreate(
		inPropertyHandle->parent(),
		QMetaType::fromType<QVector3D>(),
		inPropertyHandle->createSubPath("Position"),
		[inPropertyHandle]() {
			return QVector3D();
		},
		[inPropertyHandle](QVariant var) {
		}
	);
	inBuilder->addProperty(position);

	QPropertyHandle* rotation = QPropertyHandle::FindOrCreate(
		inPropertyHandle->parent(),
		QMetaType::fromType<QVector3D>(),
		inPropertyHandle->createSubPath("Rotation"),
		[inPropertyHandle]() {
			return QVector3D();
		},
		[inPropertyHandle](QVariant var) {
		}
	);
	inBuilder->addProperty(rotation);

	QPropertyHandle* scale = QPropertyHandle::FindOrCreate(
		inPropertyHandle->parent(),
		QMetaType::fromType<QVector3D>(),
		inPropertyHandle->createSubPath("Scale"),
		[inPropertyHandle]() {
			return QVector3D();
		},
		[inPropertyHandle](QVariant var) {
		}
	);
	inBuilder->addProperty(scale);
}


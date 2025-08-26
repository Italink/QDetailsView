#include <QApplication>
#include "QDetailsView.h"
#include "CustomObject.h"
#include "CustomType.h"
#include "PropertyTypeCustomization_CustomType.h"
#include "QQuickDetailsViewMananger.h"

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	QCustomObject obj;
	obj.setSubCustomObject(new QCustomObject);
	obj.getSubCustomObject()->setSubCustomObject(new QCustomObject);
	QQuickDetailsViewManager::Get()->registerCustomPropertyTypeLayout<QCustomType, PropertyTypeCustomization_CustomType>();

	QDetailsView view;
	view.setObject(&obj);
	view.show();
	return app.exec();
}
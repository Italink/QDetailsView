#include "QApplication"
#include "TestObject.h"
#include "QDetailsView.h"
#include <QtQuickControls2/QQuickStyle>

int main(int argc, char** argv) {
	QApplication app(argc, argv);
	QQuickStyle::setStyle("Fusion");
	TestObject obj;
	QDetailsView view;
	view.setObject(&obj);
	view.show();
	return app.exec();
}
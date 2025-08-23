#include "QApplication"
#include "TestObject.h"
#include "QDetailsView.h"
#include <QtQuickControls2/QQuickStyle>

int main(int argc, char** argv) {
	QApplication app(argc, argv);
	TestObject obj;
	QDetailsView view;
	for (int i = 0; i < 1000; i++) {
		obj.ColorList.push_back(QColor());
	}
	view.setObject(&obj);
	view.show();
	return app.exec();
}
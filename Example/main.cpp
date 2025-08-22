#include "QApplication"
#include "TestObject.h"
#include "QDetailsView.h"

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	TestObject obj;
	QDetailsView view;
	view.setObject(&obj);
	view.show();
	return app.exec();
}
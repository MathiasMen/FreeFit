#include <QApplication>
#include <QStyleFactory>

#include "include/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	MainWindow m("${CMAKE_SOURCE_DIR}/test/input/ProfileEditorReadInTest.xml");
	m.run();
	a.exec();
}
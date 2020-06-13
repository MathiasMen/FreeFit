#include <QApplication>
#include <QStyleFactory>

#include "include/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	MainWindow m("/Users/mathias/Documents/programming_workspace/FreeFit/test/input/ProfileEditorReadInTest.xml");
	m.run();
	a.exec();
}
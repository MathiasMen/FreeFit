#include <QApplication>
#include <QStyleFactory>

#include "include/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QApplication::setStyle(QStyleFactory::create("Fusion"));

	MainWindow m("/Users/mathias/Documents/programming_workspace/FreeFit/test/ProfileEditorReadInTest.xml");
	m.run();
	a.exec();
}
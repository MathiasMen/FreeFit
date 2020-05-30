#include <QApplication>

#include "include/mainwindow.h"
#include "include/profileeditor.h"
#include "include/exerciseeditor.h"
#include "include/workoutwidget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow w;
	w.show();
	FreeFit::GUI::ProfileEditor* p = new FreeFit::GUI::ProfileEditor("/Users/mathias/Documents/programming_workspace/FreeFit/test/ProfileEditorReadInTest.xml");
	p->exec();
	FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p->getCurrentlySelectedData());
	e->exec();
	FreeFit::GUI::WorkoutWidget* w = new FreeFit::GUI::WorkoutWidget();
	w->show();
	a.exec();
}
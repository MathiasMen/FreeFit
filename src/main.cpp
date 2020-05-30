#include <QApplication>

#include <iostream>

#include "include/profileeditor.h"
#include "include/exerciseeditor.h"
#include "include/workoutwidget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	FreeFit::GUI::ProfileEditor* p = new FreeFit::GUI::ProfileEditor("/Users/mathias/Documents/programming_workspace/FreeFit/test/ProfileEditorReadInTest.xml");
	p->exec();
	FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p->getCurrentlySelectedData());
	e->exec();
	FreeFit::Data::AllExercisesWorkout* w = new FreeFit::Data::AllExercisesWorkout(e->getExerciseData());
	w->generate();
	FreeFit::GUI::WorkoutWidget* ww = new FreeFit::GUI::WorkoutWidget(w);
	ww->show();
	a.exec();
}
#include <QApplication>
#include <QStyleFactory>

#include <iostream>

#include "include/profileeditor.h"
#include "include/exerciseeditor.h"
#include "include/workoutgenerationwidget.h"
#include "include/workoutwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	FreeFit::GUI::ProfileEditor* p = new FreeFit::GUI::ProfileEditor("/Users/mathias/Documents/programming_workspace/FreeFit/test/ProfileEditorReadInTest.xml");
	p->exec();
	FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p->getCurrentlySelectedData());
	e->exec();
    FreeFit::GUI::WorkoutGenerationWidget* w = new FreeFit::GUI::WorkoutGenerationWidget;
    w->setPossibleExercises(e->getExerciseData());
	w->exec();
	FreeFit::GUI::WorkoutWidget* ww = new FreeFit::GUI::WorkoutWidget(w->getSelectedWorkout()->getWorkout().get());
	ww->show();
	a.exec();
}
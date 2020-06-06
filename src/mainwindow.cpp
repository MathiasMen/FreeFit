#include "include/mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
}

void MainWindow::run()
{
	p = new FreeFit::GUI::ProfileEditor("/Users/mathias/Documents/programming_workspace/FreeFit/test/ProfileEditorReadInTest.xml");
	p->exec();
	e = new FreeFit::GUI::ExerciseEditor(p->getCurrentlySelectedData());
	e->exec();
    w = new FreeFit::GUI::WorkoutGenerationWidget;
    w->setPossibleExercises(e->getExerciseData());
	w->exec();
	ww = new FreeFit::GUI::WorkoutWidget(w->getSelectedWorkout()->getWorkout().get());
	ww->show();

//	m_w->addWidget(p);
//	m_w->show();
}

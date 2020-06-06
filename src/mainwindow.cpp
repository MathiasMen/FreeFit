#include "include/mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), p(nullptr),e(nullptr),w(nullptr),ww(nullptr)
{
}

void MainWindow::run()
{
    reInitProfileEditor("/Users/mathias/Documents/programming_workspace/FreeFit/test/ProfileEditorReadInTest.xml");
	p->exec();
    reInitExerciseEditor();
    e->exec();
    reInitWorkoutGenerationWidget();
    w->exec();
    reInitWorkoutWidget();
    ww->show();
}

void MainWindow::reInitProfileEditor(std::string profile_path)
{
    if (p != nullptr)
        delete p;
	p = new FreeFit::GUI::ProfileEditor(profile_path);
}

void MainWindow::reInitExerciseEditor()
{
    if (e != nullptr)
        delete e;
	e = new FreeFit::GUI::ExerciseEditor(p->getCurrentlySelectedData());
}

void MainWindow::reInitWorkoutGenerationWidget()
{
    if (w != nullptr)
        delete w;
    w = new FreeFit::GUI::WorkoutGenerationWidget;
    w->setPossibleExercises(e->getExerciseData());
}

void MainWindow::reInitWorkoutWidget()
{
    if (ww != nullptr)
        delete ww;
	ww = new FreeFit::GUI::WorkoutWidget(w->getSelectedWorkout()->getWorkout().get());
}

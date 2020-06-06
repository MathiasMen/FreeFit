#include "include/mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), p(nullptr),e(nullptr),w(nullptr),ww(nullptr)
{
}

void MainWindow::run()
{
    this->show();
    presentProfileEditor("/Users/mathias/Documents/programming_workspace/FreeFit/test/ProfileEditorReadInTest.xml");
}

void MainWindow::presentProfileEditor(std::string profile_path)
{
    reInitProfileEditor(profile_path);
    connect(p,SIGNAL(accepted()),this,SLOT(presentExerciseEditor()));
    this->setCentralWidget(p);
    p->exec();
}

void MainWindow::presentExerciseEditor()
{
    reInitExerciseEditor();
    connect(e,SIGNAL(accepted()),this,SLOT(presentWorkoutGenerationWidget()));
    this->setCentralWidget(e);
    e->exec();
}

void MainWindow::presentWorkoutGenerationWidget()
{
    reInitWorkoutGenerationWidget();
    connect(w,SIGNAL(accepted()),this,SLOT(presentWorkoutWidget()));
    this->setCentralWidget(w);
    w->exec();
}

void MainWindow::presentWorkoutWidget()
{
    reInitWorkoutWidget();
    this->setCentralWidget(ww);
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

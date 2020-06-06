#include "include/mainwindow.h"

MainWindow::MainWindow(std::string t_profile_path, QWidget *parent) :
    QMainWindow(parent), p(nullptr),e(nullptr),w(nullptr),ww(nullptr), profile_path(t_profile_path)
{
}

void MainWindow::run()
{
    this->show();
    presentProfileEditor();
}

void MainWindow::presentProfileEditor()
{
    reInitProfileEditor();
    connect(p,SIGNAL(accepted()),this,SLOT(presentExerciseEditor()));
    this->setCentralWidget(p);
    p->exec();
    disconnect(p,SIGNAL(accepted()),this,SLOT(presentExerciseEditor()));
}

void MainWindow::presentExerciseEditor()
{
    reInitExerciseEditor();
    connect(e,SIGNAL(accepted()),this,SLOT(presentWorkoutGenerationWidget()));
    connect(e,SIGNAL(rejected()),this,SLOT(presentProfileEditor()));
    this->setCentralWidget(e);
    e->exec();
    disconnect(e,SIGNAL(accepted()),this,SLOT(presentWorkoutGenerationWidget()));
    disconnect(e,SIGNAL(rejected()),this,SLOT(presentProfileEditor()));
}

void MainWindow::presentWorkoutGenerationWidget()
{
    reInitWorkoutGenerationWidget();
    connect(w,SIGNAL(accepted()),this,SLOT(presentWorkoutWidget()));
    connect(w,SIGNAL(rejected()),this,SLOT(presentExerciseEditor()));
    this->setCentralWidget(w);
    w->exec();
    disconnect(w,SIGNAL(accepted()),this,SLOT(presentWorkoutWidget()));
    disconnect(w,SIGNAL(rejected()),this,SLOT(presentExerciseEditor()));
}

void MainWindow::presentWorkoutWidget()
{
    reInitWorkoutWidget();
    this->setCentralWidget(ww);
    ww->show();
}

void MainWindow::reInitProfileEditor()
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

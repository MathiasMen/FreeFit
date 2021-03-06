#include "include/mainwindow.h"

MainWindow::MainWindow(std::string t_profile_path, QWidget *parent) :
    QMainWindow(parent), p(nullptr),e(nullptr),w(nullptr),ww(nullptr), profile_path(t_profile_path),color("")
{
    
}

void MainWindow::run()
{
    this->show();
    this->resize(1024,640);
    presentProfileEditor();
}

void MainWindow::presentProfileEditor()
{
    this->setWindowTitle("Profile");
    reInitProfileEditor();
    connect(p,SIGNAL(accepted()),this,SLOT(presentExerciseEditor()));
    connect(p,SIGNAL(skiptToWorkoutGeneration()),this,SLOT(skipFromProfileToWorkoutType()));
    this->setCentralWidget(p);
    p->exec();
    disconnect(p,SIGNAL(accepted()),this,SLOT(presentExerciseEditor()));
}

void MainWindow::skipFromProfileToWorkoutType()
{
    reInitExerciseEditor();
    connect(e,SIGNAL(accepted()),this,SLOT(presentWorkoutGenerationWidget()));
    e->accept();
    disconnect(e,SIGNAL(accepted()),this,SLOT(presentWorkoutGenerationWidget()));
}

void MainWindow::presentExerciseEditor()
{
    color = p->getColor();
    this->setWindowTitle("Exercises");
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
    this->setWindowTitle("WorkoutType");
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
    this->setWindowTitle("Workout");
    reInitWorkoutWidget();
    this->setCentralWidget(ww);
    ww->show();
}

void MainWindow::reInitProfileEditor()
{
    if (p != nullptr)
        delete p;
	p = new FreeFit::GUI::ProfileEditor(profile_path);
    if (!color.empty())
        p->setColor(color);
}

void MainWindow::reInitExerciseEditor()
{
    if (e != nullptr)
        delete e;
	e = new FreeFit::GUI::ExerciseEditor(p->getCurrentlySelectedData());
    if (!color.empty())
        e->setColor(color);
}

void MainWindow::reInitWorkoutGenerationWidget()
{
    if (w != nullptr)
        delete w;
    w = new FreeFit::GUI::WorkoutGenerationWidget;
    w->setPossibleExercises(e->getExerciseData());
    if (!color.empty())
        w->setColor(color);
}

void MainWindow::reInitWorkoutWidget()
{
    if (ww != nullptr)
        delete ww;
	ww = new FreeFit::GUI::WorkoutWidget(w->getSelectedWorkout()->getWorkout().get());
    if (!color.empty())
        ww->setColor(color);
}

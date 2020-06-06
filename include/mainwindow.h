#pragma once

#include <QMainWindow>
#include <QStackedWidget>

#include "include/profileeditor.h"
#include "include/exerciseeditor.h"
#include "include/workoutgenerationwidget.h"
#include "include/workoutwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    void run();
private:
    QStackedWidget* m_w;
	FreeFit::GUI::ProfileEditor* p;
	FreeFit::GUI::ExerciseEditor* e;
    FreeFit::GUI::WorkoutGenerationWidget* w;
	FreeFit::GUI::WorkoutWidget* ww;
};
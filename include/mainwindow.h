#pragma once

#include <string>

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
    MainWindow(std::string profile_path = "", QWidget *parent = nullptr);
    void run();
private:
    void reInitProfileEditor();
    void reInitExerciseEditor();
    void reInitWorkoutGenerationWidget();
    void reInitWorkoutWidget();

    QStackedWidget* m_w;
	FreeFit::GUI::ProfileEditor* p;
	FreeFit::GUI::ExerciseEditor* e;
    FreeFit::GUI::WorkoutGenerationWidget* w;
	FreeFit::GUI::WorkoutWidget* ww;

    std::string profile_path;
private slots:
    void presentProfileEditor();
    void presentExerciseEditor();
    void presentWorkoutGenerationWidget();
    void presentWorkoutWidget();
    void skipFromProfileToWorkoutType();
};
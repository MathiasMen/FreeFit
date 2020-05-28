#pragma once

#include <iostream>

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>

#include "include/workout.h"
#include "include/exerciselistwidget.h"
#include "include/exerciseviewer.h"

namespace FreeFit
{
    namespace GUI
    {
        class WorkoutWidgetTimer : public QWidget
        {
            Q_OBJECT
        public:
            WorkoutWidgetTimer(QWidget* parent) : QWidget(parent)
            {
                ly = new QHBoxLayout(this);
                exercise_duration_timer = new QTimer(this);
                update_interval_timer = new QTimer(this);
                update_interval_timer->setInterval(20);
                time_label = new QLabel(this);
            }

            void startTimer(unsigned int seconds)
            {
                exercise_duration_timer->setInterval(seconds*1000);
                exercise_duration_timer->setSingleShot(true);
                connect(exercise_duration_timer,&QTimer::timeout,this,&WorkoutWidgetTimer::timerEnded);
                connect(update_interval_timer,&QTimer::timeout,this,&WorkoutWidgetTimer::updateLabel);
                exercise_duration_timer->start();
                update_interval_timer->start();
            }

            void setDefaultLabelText(){time_label->setText("-/-");}
            
            void setTimeText(int milliseconds)
            {   
                time_label->setText(QString::number(milliseconds/1000.0,'f',1));
            }

        private slots:
            void timerEnded()
            {
                disconnect(exercise_duration_timer,&QTimer::timeout,this,&WorkoutWidgetTimer::timerEnded);
                disconnect(update_interval_timer,&QTimer::timeout,this,&WorkoutWidgetTimer::updateLabel);
                exercise_duration_timer->stop();
                update_interval_timer->stop();
                std::cout << "Time is up!" << std::endl;
            }

            void updateLabel()
            {
                setTimeText(exercise_duration_timer->remainingTime());
            }
        private:
            QHBoxLayout* ly;
            QLabel* time_label;
            QTimer* exercise_duration_timer;
            QTimer* update_interval_timer;
        };

        class WorkoutWidgetControl : public QWidget
        {
            Q_OBJECT
        public:
            WorkoutWidgetControl(QWidget* parent = nullptr)
            {
                ly = new QHBoxLayout(this);

                recreate_button = new QPushButton("recreate",this);
                play_button = new QPushButton("play",this);
                pause_button = new QPushButton("pause",this);

                connect(recreate_button,&QPushButton::clicked,this,&WorkoutWidgetControl::recreateClicked);
                connect(play_button,&QPushButton::clicked,this,&WorkoutWidgetControl::playClicked);
                connect(pause_button,&QPushButton::clicked,this,&WorkoutWidgetControl::pauseClicked);

                ly->addWidget(recreate_button);
                ly->addWidget(play_button);
                ly->addWidget(pause_button);
            }
        signals:
            void recreateClicked();
            void playClicked();
            void pauseClicked();
        private:
            QHBoxLayout* ly;

            QPushButton* recreate_button;
            QPushButton* play_button;
            QPushButton* pause_button;
        };

        class WorkoutWidget : public QWidget
        {
            Q_OBJECT
        public:
            WorkoutWidget(FreeFit::Data::WorkoutBase* t_w, QWidget* t_p = nullptr) : QWidget(t_p),w(t_w) 
            {
                ly = new QGridLayout(this);

                exercise_list = new FreeFit::GUI::ExerciseListWidget(this);
                exercise_view = new FreeFit::GUI::Exerciseviewer(this);
                control = new WorkoutWidgetControl(this);
                timer = new WorkoutWidgetTimer(this);

                exercise_list->generateWidgets(w);
                exercise_list->setMinimumWidth(320);
                
                exercise_view->set_media(QString::fromStdString(w->getExercisesPerRound().begin()->getVideoPath()));
                exercise_view->setMinimumSize(640,360);

                control->setMinimumWidth(320);
                connect(control,&WorkoutWidgetControl::recreateClicked,this,&WorkoutWidget::recreateClicked);
                connect(control,&WorkoutWidgetControl::playClicked,this,&WorkoutWidget::playClicked);
                connect(control,&WorkoutWidgetControl::pauseClicked,this,&WorkoutWidget::pauseClicked);

                ly->addWidget(exercise_list,0,0,8,1);
                ly->addWidget(control,8,0,2,1);
                ly->addWidget(exercise_view,0,1,8,1);
                ly->addWidget(timer,8,1,2,1,Qt::AlignCenter);

                this->setLayout(ly);
            }
            void setWorkout(FreeFit::Data::WorkoutBase* t_w){w = t_w;}

        private slots:
            void recreateClicked()
            {
                std::cout << "Recreate!" << std::endl;
            }

            void playClicked()
            {
                exercise_view->start();
                timer->startTimer(30);
            }

            void pauseClicked()
            {
                exercise_view->pause();
            }
        private:
            QGridLayout* ly;

            FreeFit::GUI::ExerciseListWidget* exercise_list;
            FreeFit::GUI::Exerciseviewer* exercise_view;
            FreeFit::GUI::WorkoutWidgetControl* control;
            FreeFit::GUI::WorkoutWidgetTimer* timer;

            FreeFit::Data::WorkoutBase* w;
        };
    }
}
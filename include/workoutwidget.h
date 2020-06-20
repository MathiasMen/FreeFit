#pragma once

#include <iostream>

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>

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
                setDefaultLabelText();
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

            void stop()
            {
                disconnect(exercise_duration_timer,&QTimer::timeout,this,&WorkoutWidgetTimer::timerEnded);
                disconnect(update_interval_timer,&QTimer::timeout,this,&WorkoutWidgetTimer::updateLabel);
                exercise_duration_timer->stop();
                update_interval_timer->stop();
            }
        signals:
            void exerciseTimeEnded();
        private slots:
            void timerEnded()
            {
                stop();
                emit exerciseTimeEnded();
            }

            void updateLabel()
            {
                setTimeText(exercise_duration_timer->remainingTime());
            }
        private:
            void setDefaultLabelText(){time_label->setText("-/-");}
            
            void setTimeText(int milliseconds)
            {   
                time_label->setText(QString::number(milliseconds/1000.0,'f',0));
            }

            QHBoxLayout* ly;
            QLabel* time_label;
            QTimer* exercise_duration_timer;
            QTimer* update_interval_timer;

            bool is_only_paused = false;
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

                connect(recreate_button,&QPushButton::clicked,this,&WorkoutWidgetControl::recreateClicked);
                connect(play_button,&QPushButton::clicked,this,&WorkoutWidgetControl::playClicked);

                ly->addWidget(recreate_button);
                ly->addWidget(play_button);
            }
        signals:
            void recreateClicked();
            void playClicked();
        private:
            QHBoxLayout* ly;

            QPushButton* recreate_button;
            QPushButton* play_button;
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

                timer->setMinimumWidth(640);
                connect(timer,&WorkoutWidgetTimer::exerciseTimeEnded,this,&WorkoutWidget::nextExercise);

                ly->addWidget(exercise_list,0,0,8,1);
                ly->addWidget(control,8,0,2,1);
                ly->addWidget(exercise_view,0,1,8,1);
                ly->addWidget(timer,8,1,2,1,Qt::AlignCenter);

                this->setLayout(ly);
            }
            void setWorkout(FreeFit::Data::WorkoutBase* t_w){w = t_w;}

        private slots:
            void nextExercise()
            {
                if (exercise_list->advanceCurrentExercise())
                {
                    timer->startTimer(exercise_list->getLengthOfCurrentExercise());
                    exercise_view->set_media(QString::fromStdString(exercise_list->getVideoPathOfCurrentExercise()));
                    exercise_view->start();
                }
                else
                    workoutFinished();
            }

            void workoutFinished()
            {
                disconnect(exercise_view,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(triggerReplay(QMediaPlayer::State)));
                exercise_view->stop();
                timer->stop();
                QMessageBox msg;
                msg.setText("Workout finished!");
                msg.exec();
            }

            void recreateClicked()
            {
                std::cout << "Recreate!" << std::endl;
            }

            void playClicked()
            {
                exercise_view->start();
                timer->startTimer(exercise_list->getLengthOfCurrentExercise());
                disconnect(control,&WorkoutWidgetControl::playClicked,this,&WorkoutWidget::playClicked);
                connect(exercise_view,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(triggerReplay(QMediaPlayer::State)));
            }

            void triggerReplay(QMediaPlayer::State s)
            {
                if (s == QMediaPlayer::StoppedState)
                    exercise_view->start();
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
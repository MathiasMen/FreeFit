#pragma once

#include <iostream>

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
#include <QSound>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>

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
            WorkoutWidgetTimer(int start_time, QWidget* parent) : QWidget(parent),
                line_width(5), angle_factor(16), start_angle(90*angle_factor), current_time(start_time)
            {
                exercise_duration_timer = new QTimer(this);
                notification_timer = new QTimer(this);
                update_interval_timer = new QTimer(this);
                update_interval_timer->setInterval(20);
                this->setMinimumSize(100,100);
                this->setMaximumSize(100,100);
            }

            int getRemainingTime(){return int(exercise_duration_timer->remainingTime()/1000);}

            void startTimer(unsigned int seconds)
            {
                current_time = seconds;
                exercise_duration_timer->setInterval(seconds*1000);
                exercise_duration_timer->setSingleShot(true);
                notification_timer->setInterval((seconds-3)*1000);
                notification_timer->setSingleShot(true);
                connect(exercise_duration_timer,&QTimer::timeout,this,&WorkoutWidgetTimer::timerEnded);
                connect(update_interval_timer, &QTimer::timeout, this, QOverload<>::of(&WorkoutWidgetTimer::update));  
                connect(update_interval_timer, &QTimer::timeout, this, &WorkoutWidgetTimer::updateCurrentTime);                
                connect(notification_timer,&QTimer::timeout,this,&WorkoutWidgetTimer::notificationTimerEnded);
                exercise_duration_timer->start();
                notification_timer->start();
                update_interval_timer->start();
            }

            void disconnectTimers()
            {
                disconnect(exercise_duration_timer,&QTimer::timeout,this,&WorkoutWidgetTimer::timerEnded);
                disconnect(update_interval_timer, &QTimer::timeout, this, QOverload<>::of(&WorkoutWidgetTimer::update));  
                disconnect(update_interval_timer, &QTimer::timeout, this, &WorkoutWidgetTimer::updateCurrentTime);                
                disconnect(notification_timer,&QTimer::timeout,this,&WorkoutWidgetTimer::notificationTimerEnded);
            }

            void stopTimers()
            {
                exercise_duration_timer->stop();
                notification_timer->stop();
                update_interval_timer->stop();
            }

            void stop()
            {
                current_time = 0;
                disconnectTimers();
                stopTimers();
            }

        protected:
            void paintEvent(QPaintEvent* ev) override
            {
                QPainter painter(this);
                painter.setPen(QPen(QBrush(Qt::green),line_width));
                QFont f = painter.font();
                f.setPixelSize(36);
                painter.setFont(f);
                QRect bounds = this->rect();
                span_angle = (current_time*360)/60*angle_factor;
                painter.drawArc(bounds.x()+line_width,bounds.y()+line_width,bounds.width()-2*line_width,bounds.height()-2*line_width,start_angle,span_angle);
                painter.drawText(bounds,Qt::AlignCenter,QString::number(current_time));
            }
        signals:
            void exerciseTimeEnded();
            void notificationTimerEnded();
        private slots:
            void timerEnded()
            {
                stop();
                emit exerciseTimeEnded();
            }

            void updateCurrentTime()
            {
                current_time = exercise_duration_timer->remainingTime()/1000;
            }

        private:
            QTimer* exercise_duration_timer;
            QTimer* notification_timer;
            QTimer* update_interval_timer;

            const int angle_factor;
            const int line_width;
            const int start_angle;
            int current_time;
            int span_angle;

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
                ly = new QHBoxLayout(this);

                QGridLayout* left_ly = new QGridLayout;
                QGridLayout* right_ly = new QGridLayout;

                exercise_list = new FreeFit::GUI::ExerciseListWidget(this);
                control = new WorkoutWidgetControl(this);
                left_ly->addWidget(exercise_list,0,0);
                left_ly->addWidget(control,1,0);
                
                exercise_view = new FreeFit::GUI::Exerciseviewer(this);
                timer = new WorkoutWidgetTimer(w->getExercisesPerRound().begin()->getLength(),this);
                right_ly->addWidget(exercise_view,0,0,1,3);
                right_ly->addWidget(timer,1,1,Qt::AlignHCenter);

                exercise_list->generateWidgets(w);
                exercise_list->setMinimumWidth(480);

                exercise_view->set_media(QString::fromStdString(w->getExercisesPerRound().begin()->getVideoPath()));
                exercise_view->setMinimumSize(640,360);

                control->setMinimumWidth(320);
                connect(control,&WorkoutWidgetControl::recreateClicked,this,&WorkoutWidget::recreateClicked);
                connect(control,&WorkoutWidgetControl::playClicked,this,&WorkoutWidget::playClicked);

                exercise_view->setMinimumWidth(640);
                connect(timer,&WorkoutWidgetTimer::exerciseTimeEnded,this,&WorkoutWidget::nextExercise);
                connect(timer,&WorkoutWidgetTimer::notificationTimerEnded,this,&WorkoutWidget::playNotificationSound);

                ly->addLayout(left_ly);
                ly->addLayout(right_ly);

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
            {}

            void playClicked()
            {
                int t = (memory_exercise_time != 0 ? memory_exercise_time : exercise_list->getLengthOfCurrentExercise());
                exercise_view->start();
                timer->startTimer(t);
                disconnect(control,&WorkoutWidgetControl::playClicked,this,&WorkoutWidget::playClicked);
                connect(control,&WorkoutWidgetControl::playClicked,this,&WorkoutWidget::pauseClicked);
                connect(exercise_view,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(triggerReplay(QMediaPlayer::State)));
                memory_exercise_time = 0;
            }

            void pauseClicked()
            {
                memory_exercise_time = timer->getRemainingTime();
                disconnect(exercise_view,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(triggerReplay(QMediaPlayer::State)));
                disconnect(control,&WorkoutWidgetControl::playClicked,this,&WorkoutWidget::pauseClicked);
                exercise_view->pause();
                timer->stop();
                connect(control,&WorkoutWidgetControl::playClicked,this,&WorkoutWidget::playClicked);
            }

            void triggerReplay(QMediaPlayer::State s)
            {
                if (s == QMediaPlayer::StoppedState)
                    exercise_view->start();
            }

            void playNotificationSound()
            {
                QSound::play("/Users/mathias/Documents/programming_workspace/FreeFit/tools/notification.wav");
            }
        private:
            QHBoxLayout* ly;
            FreeFit::GUI::ExerciseListWidget* exercise_list;
            FreeFit::GUI::Exerciseviewer* exercise_view;
            FreeFit::GUI::WorkoutWidgetControl* control;
            FreeFit::GUI::WorkoutWidgetTimer* timer;

            FreeFit::Data::WorkoutBase* w;

            int memory_exercise_time;
        };
    }
}
#pragma once

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
#include "include/materialdialog.h"
#include "include/iconbutton.h"

namespace FreeFit
{
    namespace GUI
    {
        class WorkoutWidgetValidator;

        class WorkoutWidgetTimer : public QWidget
        {
            Q_OBJECT
            friend WorkoutWidgetValidator;
        public:
            WorkoutWidgetTimer(int start_time, QWidget* parent);

            void setColor(std::string c);

            int getRemainingTime(){return int(exercise_duration_timer->remainingTime()/1000);}

            int getRemainingTimeNotification(){return int(notification_timer->remainingTime()/1000);}

            void startTimer(unsigned int seconds);

            void disconnectTimers();

            void stopTimers();

            void stop();

        protected:
            void paintEvent(QPaintEvent* ev) override;
        signals:
            void exerciseTimeEnded();
            void notificationTimerEnded();
        private slots:
            void timerEnded();

            void updateCurrentTime();

        private:
            std::string color;

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
            friend WorkoutWidgetValidator;
        public:
            WorkoutWidgetControl(QWidget* parent = nullptr);

            void showPlayIcon();

            void showPauseIcon();

            void setColor(std::string c);

        signals:
            void playPauseClicked();
        private:
            QHBoxLayout* ly;

            QIcon play_icon;
            QIcon pause_icon;

            IconButton* play_pause_button;
        };

        class WorkoutWidget : public MaterialDialog
        {
            Q_OBJECT
            friend WorkoutWidgetValidator;
        public:
            WorkoutWidget(FreeFit::Data::WorkoutBase* t_w, QWidget* t_p = nullptr);

            void setWorkout(FreeFit::Data::WorkoutBase* t_w){w = t_w;}

            void setPauseTime(int t_p){pause_time = t_p;}

            void setColor(std::string c);
        private slots:
            void handleExerciseEnded();

            void handlePauseEnded();

            void nextExercise();

            void workoutFinished();

            void playClicked();

            void pauseClicked();

            void triggerReplay(QMediaPlayer::State s);

            void playNotificationSound();
        private:
            QHBoxLayout* ly;
            FreeFit::GUI::ExerciseListWidget* exercise_list;
            FreeFit::GUI::Exerciseviewer* exercise_view;
            FreeFit::GUI::WorkoutWidgetControl* control;
            FreeFit::GUI::WorkoutWidgetTimer* timer;

            FreeFit::Data::WorkoutBase* w;

            int memory_exercise_time = 0;
            int pause_time = 10;
        };

        class WorkoutWidgetValidator : public QObject
        {
            Q_OBJECT
            public:
                WorkoutWidgetValidator(WorkoutWidget* t_w) : ww(t_w){}
                
                void clickPlayButton(){ww->control->play_pause_button->click();}

                int remainingTimeExercise(){return ww->timer->getRemainingTime();}

                int remainingTimeNotification(){return ww->timer->getRemainingTimeNotification();}
            private:
                WorkoutWidget* ww;
        };
    }
}
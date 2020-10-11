#include "include/workoutwidget.h"

namespace FreeFit
{
    namespace GUI
    {
        WorkoutWidgetTimer::WorkoutWidgetTimer(int start_time, QWidget* parent) : QWidget(parent), color("#ff0000"),
            line_width(5), angle_factor(16), start_angle(90*angle_factor), current_time(start_time)
        {
            exercise_duration_timer = new QTimer(this);
            notification_timer = new QTimer(this);
            update_interval_timer = new QTimer(this);
            update_interval_timer->setInterval(50);
            this->setMinimumSize(100,100);
            this->setMaximumSize(100,100);
        }

        void WorkoutWidgetTimer::setColor(std::string c)
        {
            color = c;
            update();
        }

        void WorkoutWidgetTimer::startTimer(unsigned int seconds)
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

        void WorkoutWidgetTimer::disconnectTimers()
        {
            disconnect(exercise_duration_timer,&QTimer::timeout,this,&WorkoutWidgetTimer::timerEnded);
            disconnect(update_interval_timer, &QTimer::timeout, this, QOverload<>::of(&WorkoutWidgetTimer::update));  
            disconnect(update_interval_timer, &QTimer::timeout, this, &WorkoutWidgetTimer::updateCurrentTime);                
            disconnect(notification_timer,&QTimer::timeout,this,&WorkoutWidgetTimer::notificationTimerEnded);
        }

        void WorkoutWidgetTimer::stopTimers()
        {
            exercise_duration_timer->stop();
            notification_timer->stop();
            update_interval_timer->stop();
        }

        void WorkoutWidgetTimer::stop()
        {
            current_time = 0;
            disconnectTimers();
            stopTimers();
        }

        void WorkoutWidgetTimer::paintEvent(QPaintEvent* ev)
        {
            QPainter painter(this);
            painter.setPen(QPen(QBrush(QColor(QString::fromStdString(color))),line_width));
            QFont f = painter.font();
            f.setPixelSize(36);
            painter.setFont(f);
            QRect bounds = this->rect();
            span_angle = (current_time*360)/60*angle_factor;
            painter.drawArc(bounds.x()+line_width,bounds.y()+line_width,bounds.width()-2*line_width,bounds.height()-2*line_width,start_angle,span_angle);
            painter.drawText(bounds,Qt::AlignCenter,QString::number(current_time));
        }

        void WorkoutWidgetTimer::timerEnded()
        {
            stop();
            emit exerciseTimeEnded();
        }

        void WorkoutWidgetTimer::updateCurrentTime()
        {
            current_time = exercise_duration_timer->remainingTime()/1000;
        }
    
        WorkoutWidgetControl::WorkoutWidgetControl(QWidget* parent)
        {
            ly = new QHBoxLayout(this);

            this->setStyleSheet("border:none; padding:10px;");

            play_icon = QIcon("${CMAKE_SOURCE_DIR}/tools/play.svg");
            pause_icon = QIcon("${CMAKE_SOURCE_DIR}/tools/pause.svg");
            recreate_icon = QIcon("${CMAKE_SOURCE_DIR}/tools/recreate.svg");

            recreate_button = new IconButton(recreate_icon,"",this,40,2);
            play_pause_button = new IconButton(play_icon,"",this,40,2);

            connect(recreate_button,&QPushButton::clicked,this,&WorkoutWidgetControl::recreateClicked);
            connect(play_pause_button,&QPushButton::clicked,this,&WorkoutWidgetControl::playPauseClicked);

            ly->addWidget(recreate_button);
            ly->addWidget(play_pause_button);
        }

        void WorkoutWidgetControl::showPlayIcon()
        {
            play_pause_button->setIcon(play_icon);
        }

        void WorkoutWidgetControl::showPauseIcon()
        {
            play_pause_button->setIcon(pause_icon);
        }

        void WorkoutWidgetControl::setColor(std::string c)
        {
            recreate_button->setColor(c);
            play_pause_button->setColor(c);
        }
    
        WorkoutWidget::WorkoutWidget(FreeFit::Data::WorkoutBase* t_w, QWidget* t_p) : MaterialDialog("","","",t_p),w(t_w) 
        {
            QGridLayout* left_ly = new QGridLayout;
            QGridLayout* right_ly = new QGridLayout;

            exercise_list = new FreeFit::GUI::ExerciseListWidget(this);
            control = new WorkoutWidgetControl(this);
            left_ly->addWidget(exercise_list,0,0);
            left_ly->addWidget(control,1,0);
            
            exercise_view = new FreeFit::GUI::Exerciseviewer(this);
            timer = new WorkoutWidgetTimer(w->getExercisesPerRound().begin()->getLength(),this);
            timer->setColor(QColor(Qt::gray).name().toStdString());
            right_ly->addWidget(exercise_view,0,0,1,3);
            right_ly->addWidget(timer,1,1,Qt::AlignHCenter);

            exercise_list->generateWidgets(w);
            exercise_list->setMinimumWidth(480);

            exercise_view->set_media(QString::fromStdString(w->getExercisesPerRound().begin()->getVideoPath()));
            exercise_view->setMinimumSize(640,360);

            control->setMinimumWidth(320);
            connect(control,&WorkoutWidgetControl::recreateClicked,this,&WorkoutWidget::recreateClicked);
            connect(control,&WorkoutWidgetControl::playPauseClicked,this,&WorkoutWidget::playClicked);

            exercise_view->setMinimumWidth(640);
            connect(timer,&WorkoutWidgetTimer::exerciseTimeEnded,this,&WorkoutWidget::handleExerciseEnded);
            connect(timer,&WorkoutWidgetTimer::notificationTimerEnded,this,&WorkoutWidget::playNotificationSound);

            addLayout(left_ly,0,0);
            addLayout(right_ly,0,1);
        }

        void WorkoutWidget::setColor(std::string c)
        {
            MaterialDialog::setColor(c);
            exercise_list->setColor(c);
            timer->setColor(c);
            control->setColor(c);
        }

        void WorkoutWidget::handleExerciseEnded()
        {
            disconnect(exercise_view,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(triggerReplay(QMediaPlayer::State)));
            exercise_view->stop();
            connect(exercise_view,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(triggerReplay(QMediaPlayer::State)));
            disconnect(timer,&WorkoutWidgetTimer::exerciseTimeEnded,this,&WorkoutWidget::handleExerciseEnded);
            connect(timer,&WorkoutWidgetTimer::exerciseTimeEnded,this,&WorkoutWidget::handlePauseEnded);
            timer->startTimer(pause_time);
        }

        void WorkoutWidget::handlePauseEnded()
        {
            disconnect(timer,&WorkoutWidgetTimer::exerciseTimeEnded,this,&WorkoutWidget::handlePauseEnded);
            connect(timer,&WorkoutWidgetTimer::exerciseTimeEnded,this,&WorkoutWidget::handleExerciseEnded);
            nextExercise();
        }

        void WorkoutWidget::nextExercise()
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

        void WorkoutWidget::workoutFinished()
        {
            disconnect(exercise_view,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(triggerReplay(QMediaPlayer::State)));
            exercise_view->stop();
            timer->stop();
            QMessageBox msg;
            msg.setText("Workout finished!");
            msg.exec();
        }

        void WorkoutWidget::playClicked()
        {
            int t = (memory_exercise_time != 0 ? memory_exercise_time : exercise_list->getLengthOfCurrentExercise());
            exercise_view->start();
            timer->startTimer(t);
            disconnect(control,&WorkoutWidgetControl::playPauseClicked,this,&WorkoutWidget::playClicked);
            connect(control,&WorkoutWidgetControl::playPauseClicked,this,&WorkoutWidget::pauseClicked);
            connect(exercise_view,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(triggerReplay(QMediaPlayer::State)));
            memory_exercise_time = 0;
            control->showPauseIcon();
            timer->setColor(color);
        }

        void WorkoutWidget::pauseClicked()
        {
            memory_exercise_time = timer->getRemainingTime();
            disconnect(exercise_view,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(triggerReplay(QMediaPlayer::State)));
            disconnect(control,&WorkoutWidgetControl::playPauseClicked,this,&WorkoutWidget::pauseClicked);
            exercise_view->pause();
            timer->stop();
            connect(control,&WorkoutWidgetControl::playPauseClicked,this,&WorkoutWidget::playClicked);
            control->showPlayIcon();
            timer->setColor(QColor(Qt::gray).name().toStdString());
        }

        void WorkoutWidget::triggerReplay(QMediaPlayer::State s)
        {
            if (s == QMediaPlayer::StoppedState)
                exercise_view->start();
        }

        void WorkoutWidget::playNotificationSound()
        {
            QSound::play("${CMAKE_SOURCE_DIR}/tools/notification.wav");
        }
    
    }
}
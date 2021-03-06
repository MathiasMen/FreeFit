#pragma once

#include "include/exercise.h"

#include <QUrl>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QSize>

namespace FreeFit
{
    namespace GUI
    {
        class Exerciseviewer : public QVideoWidget
        {
            Q_OBJECT
        public:
            Exerciseviewer(QWidget* = nullptr);
            void set_media(QString);
            QMediaPlayer::State getViewerState(){return p->state();}
            void resizeToCircumventBug();
        private:
            QMediaPlayer* p;
        signals:
            void stateChanged(QMediaPlayer::State s);
        public slots:
            void start();
            void pause();
            void stop();
        };
    }
}

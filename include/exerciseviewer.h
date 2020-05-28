#pragma once

#include "include/exercise.h"

#include <QUrl>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>

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
        private:
            QMediaPlayer* p;
        public slots:
            void start();
            void pause();
            void stop();
        };
    }
}

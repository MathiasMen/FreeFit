#include "include/exerciseviewer.h"

FreeFit::GUI::Exerciseviewer::Exerciseviewer(QWidget* parent) : QVideoWidget(parent)
{
    p = new QMediaPlayer(this);
    p->setVideoOutput(this);
    p->setMuted(true);
    this->resize(500,500);
    this->show();
}

void FreeFit::GUI::Exerciseviewer::set_media(QString s)
{
    p->setMedia(QUrl::fromLocalFile(s));
}

void FreeFit::GUI::Exerciseviewer::start()
{
    p->play();
}

void FreeFit::GUI::Exerciseviewer::pause()
{
    p->pause();
}

void FreeFit::GUI::Exerciseviewer::stop()
{
    if (p->state() == QMediaPlayer::PlayingState || p->state() == QMediaPlayer::PausedState)
        p->stop();
}

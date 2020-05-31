#include "include/exerciseviewer.h"

FreeFit::GUI::Exerciseviewer::Exerciseviewer(QWidget* parent) : QVideoWidget(parent)
{
    p = new QMediaPlayer(this);
    p->setVideoOutput(this);
    p->setMuted(true);
    connect(p,SIGNAL(stateChanged(QMediaPlayer::State)),this,SIGNAL(stateChanged(QMediaPlayer::State)));
    this->show();
}

void FreeFit::GUI::Exerciseviewer::set_media(QString s)
{
    p->setMedia(QUrl::fromLocalFile(s));
}

void FreeFit::GUI::Exerciseviewer::resizeToCircumventBug()
{
    QSize old_size = this->size();
    this->resize(old_size + QSize(1,1));
    this->resize(old_size);
}

void FreeFit::GUI::Exerciseviewer::start()
{
    p->play();
    resizeToCircumventBug();
}

void FreeFit::GUI::Exerciseviewer::pause()
{
    p->pause();
    resizeToCircumventBug();
}

void FreeFit::GUI::Exerciseviewer::stop()
{
    if (p->state() == QMediaPlayer::PlayingState || p->state() == QMediaPlayer::PausedState)
        p->stop();
    resizeToCircumventBug();
}

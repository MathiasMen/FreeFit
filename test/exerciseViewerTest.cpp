#include <gtest/gtest.h>

#include <QApplication>
#include <QString>
#include <QMediaPlayer>

#include "include/exerciseviewer.h"

int my_argc;
char** my_argv;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    my_argc = argc;
    my_argv = argv;
    return RUN_ALL_TESTS();
}

class ExerciseView : public ::testing::Test
{
    protected:
        std::string video_path = "/Users/mathias/Documents/programming_workspace/FreeFit/test/Pushup.mp4";
};

TEST_F(ExerciseView,Launch)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::Exerciseviewer* v = new FreeFit::GUI::Exerciseviewer(nullptr);
}

TEST_F(ExerciseView,Play)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::Exerciseviewer* v = new FreeFit::GUI::Exerciseviewer(nullptr);
    v->set_media(QString::fromStdString(video_path));
    v->show();
    v->start();
    ASSERT_EQ(v->getViewerState(),QMediaPlayer::PlayingState);    
}

TEST_F(ExerciseView,Pause)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::Exerciseviewer* v = new FreeFit::GUI::Exerciseviewer(nullptr);
    v->set_media(QString::fromStdString(video_path));
    v->show();
    v->start();
    v->pause();
    ASSERT_EQ(v->getViewerState(),QMediaPlayer::PausedState);    
}

TEST_F(ExerciseView,Stop)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::Exerciseviewer* v = new FreeFit::GUI::Exerciseviewer(nullptr);
    v->set_media(QString::fromStdString(video_path));
    v->show();
    ASSERT_EQ(v->getViewerState(),QMediaPlayer::StoppedState);    
    v->start();
    v->stop();
    ASSERT_EQ(v->getViewerState(),QMediaPlayer::StoppedState);    
}

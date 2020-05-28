#include <gtest/gtest.h>

#include <QApplication>
#include <QString>

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
    v->set_media(QString::fromStdString(video_path));
    v->show();
    v->start();
    a.exec();
}

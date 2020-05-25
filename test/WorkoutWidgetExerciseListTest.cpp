#include <gtest/gtest.h>

#include <list>
#include <QApplication>

#include "include/exerciselistwidget.h"
#include "include/exercise.h"

int my_argc;
char** my_argv;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    my_argc = argc;
    my_argv = argv;
    return RUN_ALL_TESTS();
}

class WorkoutExerciseList : public ::testing::Test
{
    protected:
    std::list<FreeFit::Data::Exercise> e_dat;

    virtual void SetUp()
    {
        FreeFit::Data::Exercise e1,e2;
        e1.setName("Pushup");
        e2.setName("Plank");
        e_dat = std::list<FreeFit::Data::Exercise> {e1, e2};
    }
};

TEST_F(WorkoutExerciseList,Launch)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ExerciseListWidget* w = new FreeFit::GUI::ExerciseListWidget(nullptr,e_dat);
    w->show();
    a.exec();
}
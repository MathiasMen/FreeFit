#include <gtest/gtest.h>

#include <list>
#include <QApplication>

#include "include/exerciselistwidget.h"

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
    std::list<Data::ExerciseData>* e_dat;

    virtual void SetUp()
    {
        e_dat = new std::list<Data::ExerciseData>();
    }
};

TEST_F(WorkoutExerciseList,Launch)
{
    QApplication a(my_argc,my_argv);
    GUI::ExerciseListWidget* w = new GUI::ExerciseListWidget(nullptr,e_dat);
    w->show();
    a.exec();
}
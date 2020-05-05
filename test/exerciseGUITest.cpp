#include <gtest/gtest.h>

#include <QDialog>

#include "include/exerciseeditor.h"
#include "include/newexercisedemand.h"

int my_argc;
char** my_argv;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    my_argc = argc;
    my_argv = argv;
    return RUN_ALL_TESTS();
}

TEST(ExerciseEditor,LaunchEditor)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ExerciseEditor* d = new FreeFit::GUI::ExerciseEditor();
    d->exec();
}

TEST(ExerciseEditor,AddButton)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor();
    e->exec();
    FreeFit::GUI::ExerciseEditorValidator v;
    v.pushAddButton(e);
    int n_exercises = v.getNumberOfExercises(e);    
    ASSERT_EQ(n_exercises,2);
}

TEST(ExerciseEditor,ExerciseDemand)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor();
    e->exec();
    FreeFit::GUI::ExerciseEditorValidator v;
    FreeFit::GUI::NewExerciseDemand* d = v.getFirstExerciseDemand(e);
    ASSERT_EQ(d->name,"...");
    ASSERT_EQ(d->video_url,"...");
    ASSERT_EQ(d->video_start_time,"...");
    ASSERT_EQ(d->video_end_time,"...");
}

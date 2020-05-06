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
}

TEST(ExerciseEditor,AddButton)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor();
    FreeFit::GUI::ExerciseEditorValidator v;
    v.pushAddButton(e);
    int n_exercises = v.getNumberOfExercises(e);    
    ASSERT_EQ(n_exercises,2);
}

TEST(ExerciseEditor,ExerciseDemand)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor();
    FreeFit::GUI::ExerciseEditorValidator v;
    FreeFit::GUI::NewExerciseDemand* d = v.getFirstExerciseDemand(e);
    ASSERT_EQ(d->name,"...");
    ASSERT_EQ(d->video_url,"...");
    ASSERT_EQ(d->video_start_time,"...");
    ASSERT_EQ(d->video_end_time,"...");
}

TEST(ExerciseEditor,NonStandardInput)
{
    std::string ex_name = "Pushup";
    std::string ex_url = "https://www.youtube.com/watch?v=BxIUwbb1Nzg";
    std::string ex_start = "2";
    std::string ex_end = "3";
    
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor();
    FreeFit::GUI::ExerciseEditorValidator v;
    v.setFirstExerciseNameText(e,ex_name);
    v.setFirstExerciseURLText(e,ex_url);
    v.setFirstExerciseStartTimeText(e,ex_start);
    v.setFirstExerciseStopTimeText(e,ex_end);

    FreeFit::GUI::NewExerciseDemand* d = v.getFirstExerciseDemand(e);
    ASSERT_EQ(d->name,ex_name);
    ASSERT_EQ(d->video_url,ex_url);
    ASSERT_EQ(d->video_start_time,ex_start);
    ASSERT_EQ(d->video_end_time,ex_end);
}

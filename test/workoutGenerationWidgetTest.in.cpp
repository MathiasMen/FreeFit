#include <gtest/gtest.h>

#include <list>

#include <QApplication>

#include "include/exercise.h"
#include "include/datadefinitions.h"
#include "include/workoutgenerationwidget.h"

int my_argc;
char** my_argv;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    my_argc = argc;
    my_argv = argv;
    return RUN_ALL_TESTS();
}

class WorkoutGenerationWidgetTest : public ::testing::Test
{
    protected:
    std::list<FreeFit::Data::Exercise> e_dat;

    virtual void SetUp()
    {
        FreeFit::Data::Exercise e1,e2;
        e1.setName("Pushup");
        e1.setVideoURL("testURL1");
        e1.setVideoPath("testPath1");
        e1.setThumbnailPath("${CMAKE_SOURCE_DIR}/test/input/Pushup.jpg");
        e1.setVideoStartTime("0");
        e1.setVideoEndTime("1");
        e1.addTrainedMuscle(FreeFit::Data::MuscleGroup::Shoulder);
        e1.addTrainedMuscle(FreeFit::Data::MuscleGroup::Back);

        e2.setName("Plank");
        e2.setVideoURL("testURL2");
        e2.setVideoPath("testPath2");
        e2.setThumbnailPath("${CMAKE_SOURCE_DIR}/test/input/Pushup.jpg");
        e2.setVideoStartTime("2");
        e2.setVideoEndTime("3");
        e2.addTrainedMuscle(FreeFit::Data::MuscleGroup::Chest);
        e2.addTrainedMuscle(FreeFit::Data::MuscleGroup::Abs);

        e_dat = std::list<FreeFit::Data::Exercise> {e1, e2};
    }
};

TEST_F(WorkoutGenerationWidgetTest,Launch)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::WorkoutGenerationWidget* w = new FreeFit::GUI::WorkoutGenerationWidget;
    w->setPossibleExercises(e_dat);
    w->show();
    w->accept();
}

TEST_F(WorkoutGenerationWidgetTest,GenerateAllExercisesWorkout)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::WorkoutGenerationWidget* w = new FreeFit::GUI::WorkoutGenerationWidget;
    FreeFit::GUI::WorkoutGenerationWidgetValidator v(w);
    w->setPossibleExercises(e_dat);
    v.setNumberOfRounds(4);
    w->show();
    w->accept();
    ASSERT_EQ(w->getSelectedWorkout()->getExercisesPerRound().size(),2);
    ASSERT_EQ(w->getSelectedWorkout()->getRounds(),4);
}

TEST_F(WorkoutGenerationWidgetTest,SwitchWorkoutOptions)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::WorkoutGenerationWidget* w = new FreeFit::GUI::WorkoutGenerationWidget;
    FreeFit::GUI::WorkoutGenerationWidgetValidator v(w);
    ASSERT_EQ(v.getWorkoutOption(0),w->getSelectedWorkout());
    v.clickWorkoutOption(1);
    ASSERT_EQ(v.getWorkoutOption(1),w->getSelectedWorkout());
    v.clickWorkoutOption(0);
    ASSERT_EQ(v.getWorkoutOption(0),w->getSelectedWorkout());
}

TEST_F(WorkoutGenerationWidgetTest,GenerateFilteredButNothingSelected)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::WorkoutGenerationWidget* w = new FreeFit::GUI::WorkoutGenerationWidget;
    FreeFit::GUI::WorkoutGenerationWidgetValidator v(w);
    ASSERT_EQ(v.getWorkoutOption(0),w->getSelectedWorkout());
    v.clickWorkoutOption(1);
    w->setPossibleExercises(e_dat);
    v.setNumberOfRounds(4);
    w->show();
    w->accept();
    ASSERT_EQ(w->getSelectedWorkout()->getExercisesPerRound().size(),0);
    ASSERT_EQ(w->getSelectedWorkout()->getRounds(),4);
}

TEST_F(WorkoutGenerationWidgetTest,GenerateFiltered)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::WorkoutGenerationWidget* w = new FreeFit::GUI::WorkoutGenerationWidget;
    FreeFit::GUI::WorkoutGenerationWidgetValidator v(w);
    ASSERT_EQ(v.getWorkoutOption(0),w->getSelectedWorkout());
    v.clickWorkoutOption(1);
    w->setPossibleExercises(e_dat);
    v.toggleHashtag("Shoulder");
    w->show();
    w->accept();
    ASSERT_EQ(w->getSelectedWorkout()->getExercisesPerRound().begin()->getName(),"Pushup");
}

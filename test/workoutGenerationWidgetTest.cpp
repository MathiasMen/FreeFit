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
        e1.setThumbnailPath("/Users/mathias/Documents/programming_workspace/FreeFit/test/Pushup.jpg");
        e1.setVideoStartTime("0");
        e1.setVideoEndTime("1");
        e1.addTrainedMuscle(FreeFit::Data::MuscleGroup::Shoulder);
        e1.addTrainedMuscle(FreeFit::Data::MuscleGroup::Back);

        e2.setName("Plank");
        e2.setVideoURL("testURL2");
        e2.setVideoPath("testPath2");
        e2.setThumbnailPath("/Users/mathias/Documents/programming_workspace/FreeFit/test/Pushup.jpg");
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
    a.exec();
}

#include <gtest/gtest.h>

#include <QApplication>

#include "include/xmlreader.h"
#include "include/workout.h"
#include "include/workoutwidget.h"

int my_argc;
char** my_argv;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    my_argc = argc;
    my_argv = argv;
    return RUN_ALL_TESTS();
}

class WorkoutWidget : public ::testing::Test
{
    protected:
        FreeFit::Data::ExerciseXMLReader* r;
        FreeFit::Data::AllExercisesWorkout* w;
    virtual void SetUp()
    {
        r = new FreeFit::Data::ExerciseXMLReader("${CMAKE_SOURCE_DIR}/test/input/WorkoutWidgetTest.xml");
        w = new FreeFit::Data::AllExercisesWorkout(r->getExerciseList());
        w->generate();
    }
};

TEST_F(WorkoutWidget,Launch)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::WorkoutWidget* ww = new FreeFit::GUI::WorkoutWidget(w,nullptr);
    ww->open();
    ww->accept();
}

TEST_F(WorkoutWidget,TimingUpdate)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::WorkoutWidget* ww = new FreeFit::GUI::WorkoutWidget(w,nullptr);
    FreeFit::GUI::WorkoutWidgetValidator v(ww);
    v.clickPlayButton();
    ASSERT_NEAR(v.remainingTimeExercise(),40,1);    
    int last_time = 40;
    bool success = true;
    while (v.remainingTimeExercise() != 0)
        if (v.remainingTimeExercise() != last_time)
        {
            if (v.remainingTimeExercise() != last_time - 1)
                success = false;
            last_time = v.remainingTimeExercise();
        }
    ASSERT_TRUE(success);    
}

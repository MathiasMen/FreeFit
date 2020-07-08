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
    ww->show();
}

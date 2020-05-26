#include <gtest/gtest.h>

#include <list>

#include "include/workout.h"
#include "include/xmlreader.h"

int my_argc;
char** my_argv;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    my_argc = argc;
    my_argv = argv;
    return RUN_ALL_TESTS();
}

class WorkoutTestObject : public ::testing::Test
{
    protected:
        FreeFit::Data::ExerciseXMLReader* r;
        FreeFit::Data::AllExercisesWorkout* w;
    virtual void SetUp()
    {
        r = new FreeFit::Data::ExerciseXMLReader("/Users/mathias/Documents/programming_workspace/FreeFit/test/WorkoutGenerationTest.xml");
        w = new FreeFit::Data::AllExercisesWorkout(r->getExerciseList());
    }
};

TEST_F(WorkoutTestObject,Launch)
{
    w->generate();
    ASSERT_EQ(w->getExercisesPerRound().size(),2);
    ASSERT_EQ(w->getRounds(),3);
}
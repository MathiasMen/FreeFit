#include <gtest/gtest.h>

#include <list>
#include <set>

#include "include/datadefinitions.h"
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
        r = new FreeFit::Data::ExerciseXMLReader("${CMAKE_SOURCE_DIR}/test/input/WorkoutGenerationTest.xml");
        w = new FreeFit::Data::AllExercisesWorkout(r->getExerciseList());
    }
};

TEST_F(WorkoutTestObject,Launch)
{
    w->generate();
    ASSERT_EQ(w->getExercisesPerRound().size(),2);
    ASSERT_EQ(w->getRounds(),3);
}

TEST_F(WorkoutTestObject,FilteredWorkout)
{
    FreeFit::Data::FilteredByMusclesWorkout* w_filtered = new FreeFit::Data::FilteredByMusclesWorkout(r->getExerciseList());
    std::set<FreeFit::Data::MuscleGroup> selected_areas {FreeFit::Data::stringToMuscleGroup("Back")};
    w_filtered->setSelectedAreas(selected_areas);
    w_filtered->generate();
    ASSERT_EQ(w_filtered->getExercisesPerRound().size(),1);
    ASSERT_EQ(w_filtered->getRounds(),3);
}
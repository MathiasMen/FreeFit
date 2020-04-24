#include <gtest/gtest.h>
#include "include/exercise.h"
#include "include/workout.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Datastructures, Exercise)
{
    FreeFit::Data::Exercise e;
}

TEST(Datastructures, Workout)
{
    FreeFit::Data::Workout w;
}


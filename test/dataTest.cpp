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

class DataStructureTest : public ::testing::Test
{
    protected:
        FreeFit::Data::WorkoutBase* w;

        virtual void SetUp()
        {
            FreeFit::Data::Exercise e1,e2,e3;
            e1.setName("Ex1");
            e2.setName("Ex2");
            e3.setName("Ex3");
            w = new FreeFit::Data::AllExercisesWorkout(std::list<FreeFit::Data::Exercise>{e1, e2, e3});
        }
};

TEST_F(DataStructureTest, WorkoutGeneration1)
{
    std::list<FreeFit::Data::Exercise> el = w->generate();
    ASSERT_EQ(el.size(),9);
}

TEST_F(DataStructureTest, WorkoutGeneration2)
{
    std::list<FreeFit::Data::Exercise> el = w->generate();
    ASSERT_EQ(el.begin()->getName(),"Ex1");
}

TEST_F(DataStructureTest, WorkoutGeneration3)
{
    std::list<FreeFit::Data::Exercise> el = w->generate();
    auto el_it = el.begin();
    el_it++;
    el_it++;
    el_it++;
    el_it++;
    ASSERT_EQ(el_it->getName(),"Ex2");
}



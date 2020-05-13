#include <gtest/gtest.h>
#include "include/exercise.h"
#include "include/workout.h"
#include "include/profile.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class ExerciseTest : public ::testing::Test
{
    protected:
        FreeFit::Data::Exercise e;

        virtual void SetUp()
        {
            e.addTrainedMuscle(FreeFit::Data::MuscleGroup::Shoulder);
            e.addTrainedMuscle(FreeFit::Data::MuscleGroup::Back);
            e.addTrainedMuscle(FreeFit::Data::MuscleGroup::Shoulder);
            e.addTrainedMuscle(FreeFit::Data::MuscleGroup::Legs);
        }
};

TEST_F(ExerciseTest, TrainedMusclesLength)
{
    ASSERT_EQ(e.getTrainedMuscles().size(),3);
}

TEST_F(ExerciseTest, TrainedMusclesEntry1)
{
    ASSERT_EQ(e.getTrainedMuscles().count(FreeFit::Data::MuscleGroup::Back),1);
}

TEST_F(ExerciseTest, TrainedMusclesEntry2)
{
    ASSERT_EQ(e.getTrainedMuscles().count(FreeFit::Data::MuscleGroup::Arms),0);
}

class WorkoutTest : public ::testing::Test
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

TEST_F(WorkoutTest, WorkoutGeneration1)
{
    std::list<FreeFit::Data::Exercise> el = w->generate();
    ASSERT_EQ(el.size(),9);
}

TEST_F(WorkoutTest, WorkoutGeneration2)
{
    std::list<FreeFit::Data::Exercise> el = w->generate();
    ASSERT_EQ(el.begin()->getName(),"Ex1");
}

TEST_F(WorkoutTest, WorkoutGeneration3)
{
    std::list<FreeFit::Data::Exercise> el = w->generate();
    auto el_it = el.begin();
    el_it++;
    el_it++;
    el_it++;
    el_it++;
    ASSERT_EQ(el_it->getName(),"Ex2");
}

TEST(ProfileTest, Generation)
{
    FreeFit::Data::Profile p;   
}

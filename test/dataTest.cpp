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
            e.setVideoURL("https://www.youtube.com/watch?v=-kwe1EOiWMY");
            e.setThumbnailPath("SomeDummyPath");
            e.setVideoStartTime("2");
            e.setVideoEndTime("5");
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

TEST_F(ExerciseTest, VideoURL)
{
    ASSERT_EQ(e.getVideoURL(),"https://www.youtube.com/watch?v=-kwe1EOiWMY");
}

TEST_F(ExerciseTest, ThumbnailPath)
{
    ASSERT_EQ(e.getThumbnailPath(),"SomeDummyPath");
}

TEST_F(ExerciseTest, StartTime)
{
    ASSERT_EQ(e.getVideoStartTime(),"2");
}

TEST_F(ExerciseTest, EndTime)
{
    ASSERT_EQ(e.getVideoEndTime(),"5");
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
            w->generate();
        }
};

TEST_F(WorkoutTest, WorkoutGeneration1)
{   
    std::list<FreeFit::Data::Exercise> el = w->getExercisesPerRound();
    ASSERT_EQ(el.size(),3);
    ASSERT_EQ(w->getRounds(),3);
}

TEST_F(WorkoutTest, WorkoutGeneration2)
{
    std::list<FreeFit::Data::Exercise> el = w->getExercisesPerRound();
    ASSERT_EQ(el.begin()->getName(),"Ex1");
}

TEST_F(WorkoutTest, WorkoutGeneration3)
{
    std::list<FreeFit::Data::Exercise> el = w->getExercisesPerRound();
    auto el_it = el.begin();
    el_it++;
    ASSERT_EQ(el_it->getName(),"Ex2");
}

TEST(ProfileTest, GenerationAndModification)
{
    std::string test_name = "TestName";
    std::string test_color = "TestColor";
    std::string test_pic_path = "TestPicPath";
    std::string test_date = "TestDate";
    std::string test_db_path = "TestDBPath";
    double test_performace_factor = 2.0;

    FreeFit::Data::Profile p;   
    p.setName(test_name);
    p.setColor(test_color);
    p.setPicturePath(test_pic_path);
    p.setDateLastWorkout(test_date);
    p.setPathToExerciseDB(test_db_path);
    p.setPerformanceFactor(test_performace_factor);

    ASSERT_EQ(test_name,p.getName());
    ASSERT_EQ(test_color,p.getColor());
    ASSERT_EQ(test_pic_path,p.getPicturePath());
    ASSERT_EQ(test_date,p.getDateLastWorkout());
    ASSERT_EQ(test_db_path,p.getPathToExerciseDB());
    ASSERT_EQ(test_performace_factor,p.getPerformanceFactor());
}

#pragma once

#include <string>
#include <list>

#include "include/exercise.h"

namespace FreeFit
{
    namespace Data
    {
        class WorkoutBase
        {
            public:
                WorkoutBase(std::list<Exercise> t_possible_exercises);
                
                virtual std::list<Exercise> generate() = 0;
                virtual std::list<MuscleGroup> getMainlyTrainedMuscles() = 0;
            protected:
                std::list<Exercise> possible_exercises;
                std::list<MuscleGroup> trained_areas;
                WorkoutBase();                
        };

        class AllExercisesWorkout : public WorkoutBase
        {
            public:
                AllExercisesWorkout(std::list<Exercise> t_possible_exercises);

                std::list<Exercise> generate() override;
                std::list<MuscleGroup> getMainlyTrainedMuscles() override;
            private:
                AllExercisesWorkout();
        };
    }
}

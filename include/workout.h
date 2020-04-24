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
                WorkoutBase(std::list<Exercise> t_possible_exercises) : 
                    possible_exercises(t_possible_exercises){}
                
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
                AllExercisesWorkout(std::list<Exercise> t_possible_exercises) : WorkoutBase(t_possible_exercises){}

                std::list<Exercise> generate() override
                {
                    std::list<Exercise> l;
                    unsigned int reps = 3;
                    for (unsigned int i = 0; i < reps; i++)
                        for(Exercise e : possible_exercises)
                            l.push_back(e);
                    return l;
                }

                std::list<MuscleGroup> getMainlyTrainedMuscles() override
                {
                    return std::list<MuscleGroup>{MuscleGroup::Shoulder};
                }
            private:
                AllExercisesWorkout();
        };
    }
}

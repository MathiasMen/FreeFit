#include "include/workout.h"

namespace FreeFit
{
    namespace Data
    {
        WorkoutBase::WorkoutBase(std::list<Exercise> t_possible_exercises) : possible_exercises(t_possible_exercises){}


        AllExercisesWorkout::AllExercisesWorkout(std::list<Exercise> t_possible_exercises) : WorkoutBase(t_possible_exercises){}
        
        std::list<Exercise> AllExercisesWorkout::generate()
        {
            std::list<Exercise> l;
            unsigned int reps = 3;
            for (unsigned int i = 0; i < reps; i++)
                for(Exercise e : possible_exercises)
                    l.push_back(e);
            return l;
        }

        std::list<MuscleGroup> AllExercisesWorkout::getMainlyTrainedMuscles()
        {
            return std::list<MuscleGroup>{MuscleGroup::Shoulder};
        }

    }
}
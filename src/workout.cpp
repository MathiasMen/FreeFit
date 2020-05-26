#include "include/workout.h"

namespace FreeFit
{
    namespace Data
    {
        WorkoutBase::WorkoutBase(std::list<Exercise> t_possible_exercises, unsigned int t_rounds) : possible_exercises(t_possible_exercises),rounds(t_rounds){}

        AllExercisesWorkout::AllExercisesWorkout(std::list<Exercise> t_possible_exercises) : WorkoutBase(t_possible_exercises,3)
        {

        }
        
        void AllExercisesWorkout::generate()
        {
            for(Exercise e : possible_exercises)
                exercises_per_round.push_back(e);            
        }

        std::list<MuscleGroup> AllExercisesWorkout::getMainlyTrainedMuscles()
        {
            return std::list<MuscleGroup>{MuscleGroup::Shoulder};
        }

    }
}
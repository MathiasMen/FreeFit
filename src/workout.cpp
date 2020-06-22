#include "include/workout.h"

namespace FreeFit
{
    namespace Data
    {
        WorkoutBase::WorkoutBase(std::list<Exercise> t_possible_exercises, unsigned int t_rounds) : possible_exercises(t_possible_exercises),rounds(t_rounds){}

        AllExercisesWorkout::AllExercisesWorkout(std::list<Exercise> t_possible_exercises) : WorkoutBase(t_possible_exercises,3)
        {

        }
        
        void WorkoutBase::generate()
        {
            generateExercises();
            generateExerciseTimes();
        }

        void AllExercisesWorkout::generateExercises()
        {
            for(Exercise e : possible_exercises)
                exercises_per_round.push_back(e);            
        }
        
        void AllExercisesWorkout::generateExerciseTimes()
        {
            for(auto e : exercises_per_round)
                seconds_per_exercise.push_back(30);
        }

        std::list<MuscleGroup> AllExercisesWorkout::getMainlyTrainedMuscles()
        {
            return std::list<MuscleGroup>{MuscleGroup::Shoulder};
        }

    }
}
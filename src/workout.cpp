#include "include/workout.h"

namespace FreeFit
{
    namespace Data
    {
        WorkoutBase::WorkoutBase(std::list<Exercise> t_possible_exercises, unsigned int t_rounds) : possible_exercises(t_possible_exercises),rounds(t_rounds),time_per_exercise(30){}

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
                if (exercises_per_round.size() < max_no_exercises_per_round)
                    exercises_per_round.push_back(e);
        }
        
        void AllExercisesWorkout::generateExerciseTimes()
        {
            for(auto& e : exercises_per_round)
                e.setLength(time_per_exercise);
        }

        std::list<MuscleGroup> AllExercisesWorkout::getMainlyTrainedMuscles()
        {
            return std::list<MuscleGroup>{MuscleGroup::Shoulder};
        }

        FilteredByMusclesWorkout::FilteredByMusclesWorkout(std::list<Exercise> t_possible_exercises) : WorkoutBase(t_possible_exercises,3)
        {

        }

        void FilteredByMusclesWorkout::generateExercises()
        {
            for(Exercise e : possible_exercises)
            {
                if (exercises_per_round.size() < max_no_exercises_per_round)
                {
                    bool area_match = false;
                    for(MuscleGroup m : e.getTrainedMuscles())
                        if(selected_areas.count(m) == 1)
                            area_match = true;
                    if(area_match)
                        exercises_per_round.push_back(e);            
                }
            }
        }
        
        void FilteredByMusclesWorkout::generateExerciseTimes()
        {
            for(auto& e : exercises_per_round)
                e.setLength(time_per_exercise);
        }

        void FilteredByMusclesWorkout::setSelectedAreas(std::set<MuscleGroup> a)
        {
            selected_areas = a;
        }

        void FilteredByMusclesWorkout::setSelectedAreas(std::list<std::string> areas)
        {
            std::set<MuscleGroup> areas_data;
            for (std::string a : areas)
                areas_data.insert(FreeFit::Data::stringToMuscleGroup(a));
            setSelectedAreas(areas_data);
        }

        std::list<MuscleGroup> FilteredByMusclesWorkout::getMainlyTrainedMuscles()
        {
            return std::list<MuscleGroup>{MuscleGroup::Shoulder};
        }
    }
}
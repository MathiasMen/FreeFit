#pragma once

#include <string>
#include <list>
#include <set>

#include "include/exercise.h"

namespace FreeFit
{
    namespace Data
    {
        class WorkoutBase
        {
            public:
                WorkoutBase(std::list<Exercise> t_possible_exercises,unsigned int);
                
                void generate();
                virtual std::list<MuscleGroup> getMainlyTrainedMuscles() = 0;
                virtual void generateExercises() = 0;
                virtual void generateExerciseTimes() = 0;
                void setRounds(unsigned int r){rounds = r;}
                unsigned int getRounds(){return rounds;}
                std::list<Exercise> getExercisesPerRound(){return exercises_per_round;}
                void setPossibleExercises(std::list<Exercise> t_possible_exercises){possible_exercises = t_possible_exercises;}
                bool isPossibleExercisesEmpty(){return (possible_exercises.size() == 0);}
            protected:
                std::list<Exercise> possible_exercises;
                std::list<MuscleGroup> trained_areas;
                std::list<Exercise> exercises_per_round;
                unsigned int rounds;
                WorkoutBase();                
        };

        class AllExercisesWorkout : public WorkoutBase
        {
            public:
                AllExercisesWorkout(std::list<Exercise> t_possible_exercises);

                std::list<MuscleGroup> getMainlyTrainedMuscles() override;
                void generateExercises() override;
                void generateExerciseTimes() override;
            private:
                AllExercisesWorkout();
        };

        class FilteredByMusclesWorkout : public WorkoutBase
        {
            public:
                FilteredByMusclesWorkout(std::list<Exercise> t_possible_exercises);

                std::list<MuscleGroup> getMainlyTrainedMuscles() override;
                void generateExercises() override;
                void generateExerciseTimes() override;
                void setSelectedAreas(std::set<MuscleGroup> a);
            private:
                std::set<MuscleGroup> selected_areas;
                FilteredByMusclesWorkout();
        };
    }
}

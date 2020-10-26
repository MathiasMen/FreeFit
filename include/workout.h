#pragma once

#include <string>
#include <list>
#include <set>

#include "include/exercise.h"
#include "include/datadefinitions.h"

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
                void setMaxNumberOfExercisesPerRound(unsigned int e){max_no_exercises_per_round = e;}
                void setTimeOfExercises(unsigned int t){time_per_exercise = t;}
                unsigned int getRounds(){return rounds;}
                std::list<Exercise> getExercisesPerRound(){return exercises_per_round;}
                std::list<std::string> getExercisesPerRoundNames();
                std::list<Exercise> getPossibleExercises(){return possible_exercises;}
                void setPossibleExercises(std::list<Exercise> t_possible_exercises){possible_exercises = t_possible_exercises;}
                bool isPossibleExercisesEmpty(){return (possible_exercises.size() == 0);}
                void removeExerciseByName(std::string name);
                void setName(std::string n){workout_name = n;}
                std::string getName(){return workout_name;}
            protected:
                std::string workout_name = "";
                std::list<Exercise> possible_exercises;
                std::list<MuscleGroup> trained_areas;
                std::list<Exercise> exercises_per_round;
                unsigned int rounds;
                unsigned int max_no_exercises_per_round;
                unsigned int time_per_exercise;
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
                void setSelectedAreas(std::list<std::string> areas);
            private:
                std::set<MuscleGroup> selected_areas;
                FilteredByMusclesWorkout();
        };

        class CustomExercisesWorkout : public WorkoutBase
        {
        public:
            CustomExercisesWorkout(std::list<Exercise> t_possible_exercises);

            std::list<MuscleGroup> getMainlyTrainedMuscles() override;
            void generateExercises() override;
            void generateExerciseTimes() override;
        private:
            CustomExercisesWorkout();
        };
    }
}

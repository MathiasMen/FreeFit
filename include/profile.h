#pragma once

#include <string>

namespace FreeFit
{
    namespace Data
    {
        class Profile
        {
            public:
                Profile(){};

                void setName(std::string n){name = n;}
                std::string getName(){return name;}

                void setPicturePath(std::string p){picture_path = p;}
                std::string getPicturePath(){return picture_path;}

                void setDateLastWorkout(std::string d){date_of_last_workout = d;}
                std::string getDateLastWorkout(){return date_of_last_workout;}

                void setPathToExerciseDB(std::string db){path_to_exercise_database = db;}
                std::string getPathToExerciseDB(){return path_to_exercise_database;}

                void setPerformanceFactor(double p){performance_factor = p;}
                double getPerformanceFactor(){return performance_factor;}
            private:
                std::string name;
                std::string picture_path;
                std::string date_of_last_workout;
                std::string path_to_exercise_database;
                double performance_factor;
        };
    }
}
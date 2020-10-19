#pragma once

#include <string>

namespace FreeFit
{
    namespace Data
    {
        class Profile
        {
            public:
                static Profile buildNewProfileProfile()
                {
                    return Profile("New Profile","#ff0000","","","",1.0,"");
                }

                Profile(){};
                Profile(std::string n,std::string c,std::string p_path,std::string date,std::string db_path,double perf,std::string w_db_path);

                bool isNewProfileProfile();
                void generatePathToExerciseDB()
                {
                    this->path_to_exercise_database = "${CMAKE_BINARY_DIR}/" + name + ".xml";
                }

                void generatePathToCustomWorkoutDB()
                {
                    this->path_to_custom_workouts_db = "${CMAKE_BINARY_DIR}/" + name + "_workouts.xml";
                }

                void setName(std::string n){name = n;}
                std::string getName(){return name;}

                void setColor(std::string c){color = c;}
                std::string getColor(){return color;}

                void setPicturePath(std::string p){picture_path = p;}
                std::string getPicturePath(){return picture_path;}

                void setDateLastWorkout(std::string d){date_of_last_workout = d;}
                std::string getDateLastWorkout(){return date_of_last_workout;}

                void setPathToExerciseDB(std::string db){path_to_exercise_database = db;}
                std::string getPathToExerciseDB(){return path_to_exercise_database;}

                void setPerformanceFactor(double p){performance_factor = p;}
                double getPerformanceFactor(){return performance_factor;}

                void setPathToCustomWorkoutsDB(std::string db){path_to_custom_workouts_db = db;}
                std::string getPathToCustomWorkoutsDB(){return path_to_custom_workouts_db;}
            private:
                std::string name;
                std::string color;
                std::string picture_path;
                std::string date_of_last_workout;
                std::string path_to_exercise_database;
                double performance_factor;
                std::string path_to_custom_workouts_db;
        };
    }
}
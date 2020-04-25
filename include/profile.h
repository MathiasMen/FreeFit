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
            private:
                std::string name;
                std::string picture_path;
                std::string date_of_last_workout;
                std::string path_to_exercise_database;
                double performace_factor;
        };
    }
}
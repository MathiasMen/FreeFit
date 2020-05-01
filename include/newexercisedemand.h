#pragma once

#include <string>
#include <list>

namespace FreeFit
{
    namespace GUI
    {
        struct NewExerciseDemand
        {
            std::string name;
            std::string video_url;
            std::string base_volume;
            std::string exercise_type;
            std::list<std::string> muscle_areas;
        };
    }
}
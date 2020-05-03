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
            std::string video_start_time;
            std::string video_end_time;
            std::list<std::string> muscle_areas;
        };
    }
}
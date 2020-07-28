#pragma once

#include <string>
#include <list>

namespace FreeFit
{
    namespace GUI
    {
        struct DownloadExerciseDemand
        {
            std::string name;
            std::string video_url;
            std::string video_start_time;
            std::string video_end_time;
            std::list<std::string> muscle_areas;
        };

        struct InfoExerciseDemand
        {
            std::string video_url;
        };
    }
}
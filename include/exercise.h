#pragma once

#include <string>
#include <vector>

namespace FreeFit
{
    namespace Data
    {
        enum ExerciseType {TimeBased, RepetitionBased};

        enum TrainedBodyRegions
            {Shoulder, UpperBack, MiddleBack,
            LowerBack, Chest, Biceps, Triceps,
            Forearms, CoreAbs, SideAbs, Glutes,
            Thigh, Harmstrings, Tibia, Calves};
        
        class Exercise
        {
            public:
                Exercise(){}
            private:
                std::string name;
                std::string video_path;
                unsigned int base_volume;
                ExerciseType t;
                std::vector<TrainedBodyRegions> trained_ares;
        };
    }
}


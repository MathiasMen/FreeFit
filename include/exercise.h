#pragma once

#include <string>
#include <set>

namespace FreeFit
{
    namespace Data
    {
        enum MuscleGroup
            {Shoulder, UpperBack, MiddleBack,
            LowerBack, Chest, Biceps, Triceps,
            Forearms, CoreAbs, SideAbs, Glutes,
            Thigh, Harmstrings, Tibia, Calves};
        
        class Exercise
        {
            public:
                Exercise(){}
                void setName(std::string n){name = n;}
                std::string getName(){return name;}

                void setVideoPath(std::string v){video_path = v;}
                std::string getVideoPath(){return video_path;}

                void addTrainedMuscle(MuscleGroup m){trained_areas.insert(m);};
                std::set<MuscleGroup> getTrainedMuscles(){return trained_areas;}

            private:
                std::string name;
                std::string video_path;
                std::set<MuscleGroup> trained_areas;
        };
    }
}


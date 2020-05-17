#pragma once

#include <string>
#include <set>

#include "include/datadefinitions.h"

namespace FreeFit
{
    namespace Data
    {
        class Exercise
        {
            public:
                Exercise(){}
                void setName(std::string n){name = n;}
                std::string getName(){return name;}

                void setVideoURL(std::string v){video_url = v;}
                std::string getVideoURL(){return video_url;}

                void setVideoPath(std::string v){video_path = v;}
                std::string getVideoPath(){return video_path;}

                void addTrainedMuscle(MuscleGroup m){trained_areas.insert(m);};
                std::set<MuscleGroup> getTrainedMuscles(){return trained_areas;}

            private:
                std::string name;
                std::string video_url;
                std::string video_path;
                std::set<MuscleGroup> trained_areas;
        };
    }
}


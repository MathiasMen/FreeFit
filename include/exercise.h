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

                void setThumbnailPath(std::string t){thumbnail_path = t;}
                std::string getThumbnailPath(){return thumbnail_path;}

                void setVideoStartTime(std::string t){video_start_time = t;}
                std::string getVideoStartTime(){return video_start_time;}

                void setVideoEndTime(std::string t){video_end_time = t;}
                std::string getVideoEndTime(){return video_end_time;}
                
                void addTrainedMuscle(MuscleGroup m){trained_areas.insert(m);};
                std::set<MuscleGroup> getTrainedMuscles(){return trained_areas;}

            private:
                std::string name;
                std::string video_url;
                std::string video_path;
                std::string thumbnail_path;
                std::string video_start_time;
                std::string video_end_time;
                std::set<MuscleGroup> trained_areas;
        };
    }
}


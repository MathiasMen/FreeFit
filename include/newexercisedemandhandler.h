#pragma once

#include <memory>
#include <queue>

#include "include/xmlreader.h"
#include "include/xmlwriter.h"
#include "include/newexercisedemand.h"
#include "include/exercise.h"
#include "include/youtubedownloader.h"

namespace FreeFit
{
    namespace Data
    {
        class NewExerciseDemandHandler
        {
        public:
            NewExerciseDemandHandler(std::string t_path_to_db):path_to_db(t_path_to_db)
            {
                r = std::make_shared<BaseXMLReader>(path_to_db);
                w = std::make_shared<ExerciseWriter>(path_to_db);
                yt = std::make_shared<VideoDownload::YoutubeDL>();
                yt->setVideoFormat(VideoDownload::VideoType::MP4);
            }

            void addDemand(std::shared_ptr<GUI::NewExerciseDemand> d)
            {
                demands.push(d);
            }

            void executeDemands()
            {
                w->copyNodeTree(r->read());
                std::list<FreeFit::Data::Exercise> lst;
                while(!demands.empty())
                {
                    std::shared_ptr<GUI::NewExerciseDemand> d = demands.front();
                    Exercise e;
                    e.setName(d->name);
                    for (auto a : d->muscle_areas)
                        e.addTrainedMuscle(stringToMuscleGroup(a));
                    std::string out_path = "/Users/mathias/Documents/programming_workspace/FreeFit/build/test/" + d->name + "_" + d->video_start_time + "_" + d->video_end_time + ".mp4";
                    downloadVideo(d->video_url,out_path,d->video_start_time,d->video_end_time);
                    e.setVideoPath(out_path);
                    lst.push_back(e);
                    demands.pop();
                }
                w->addExercisesToNodeTree(lst);
                w->write();
            }

        private:
            void downloadVideo(std::string url, std::string out_path, std::string start_time, std::string stop_time)
            {
                unsigned int start = std::stoi(start_time);
                unsigned int end = std::stoi(stop_time);
                if (start > 0 && end > 0 && end > start)
                    yt->download(url,out_path,start,end);
                else
                    yt->download(url,out_path);
            }

            FreeFit::Data::MuscleGroup stringToMuscleGroup(std::string s)
            {
                if (s == "Shoulder")
                    return FreeFit::Data::MuscleGroup::Shoulder;
                else if (s == "UpperBack")
                    return FreeFit::Data::MuscleGroup::UpperBack;
                else if (s == "MiddleBack")
                    return FreeFit::Data::MuscleGroup::MiddleBack;
                else if (s == "LowerBack")
                    return FreeFit::Data::MuscleGroup::LowerBack;
                else if (s == "Chest")
                    return FreeFit::Data::MuscleGroup::Chest;
                else if (s == "Biceps")
                    return FreeFit::Data::MuscleGroup::Biceps;
                else if (s == "Triceps")
                    return FreeFit::Data::MuscleGroup::Triceps;
                else if (s == "Forearms")
                    return FreeFit::Data::MuscleGroup::Forearms;
                else if (s == "CoreAbs")
                    return FreeFit::Data::MuscleGroup::CoreAbs;
                else if (s == "SideAbs")
                    return FreeFit::Data::MuscleGroup::SideAbs;
                else if (s == "Glutes")
                    return FreeFit::Data::MuscleGroup::Glutes;
                else if (s == "Thigh")
                    return FreeFit::Data::MuscleGroup::Thigh;
                else if (s == "Harmstrings")
                    return FreeFit::Data::MuscleGroup::Harmstrings;
                else if (s == "Tibia")
                    return FreeFit::Data::MuscleGroup::Tibia;
                else if (s == "Calves")
                    return FreeFit::Data::MuscleGroup::Calves;
                else
                    return FreeFit::Data::MuscleGroup::Error;
            }

            std::queue<std::shared_ptr<GUI::NewExerciseDemand>> demands;
            std::string path_to_db;
            std::shared_ptr<BaseXMLReader> r;
            std::shared_ptr<ExerciseWriter> w;
            std::shared_ptr<VideoDownload::YoutubeDL> yt;
        };
    }
}
#pragma once

#include <memory>
#include <queue>

#include "include/xmlreader.h"
#include "include/xmlwriter.h"
#include "include/xmlnode.h"
#include "include/downloadexercisedemand.h"
#include "include/youtubedownloader.h"

namespace FreeFit
{
    namespace Data
    {
        class NewExerciseDemandHandlerValidator;

        class NewExerciseDemandHandler
        {
        friend NewExerciseDemandHandlerValidator;
        public:
            NewExerciseDemandHandler(std::string t_path_to_db):path_to_db(t_path_to_db)
            {
                r = std::make_shared<BaseXMLReader>(path_to_db);
                w = std::make_shared<ExerciseWriter>(path_to_db);
                yt = std::make_shared<VideoDownload::YoutubeDL>();
                yt->setVideoFormat(VideoDownload::VideoType::MP4);
            }

            void addDemand(std::shared_ptr<GUI::DownloadExerciseDemand> d)
            {
                demands.push(d);
            }

            void executeDemands()
            {
                std::shared_ptr<FreeFit::Data::XMLNode> n = r->read();
                if(n == nullptr)
                    w->createNodeTree(std::list<FreeFit::Data::Exercise>());
                else
                    w->copyNodeTree(n);
                std::list<FreeFit::Data::Exercise> lst;
                while(!demands.empty())
                {
                    std::shared_ptr<GUI::DownloadExerciseDemand> d = demands.front();
                    Exercise e;
                    e.setName(d->name);
                    for (auto a : d->muscle_areas)
                        e.addTrainedMuscle(stringToMuscleGroup(a));
                    std::string out_path = "/Users/mathias/Documents/programming_workspace/FreeFit/build/test/" + d->name + "_" + d->video_start_time + "_" + d->video_end_time + ".mp4";
                    downloadVideo(d->video_url,out_path,d->video_start_time,d->video_end_time);
                    e.setVideoURL(d->video_url);
                    e.setVideoStartTime(d->video_start_time);
                    e.setVideoEndTime(d->video_end_time);
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
                else if (s == "Back")
                    return FreeFit::Data::MuscleGroup::Back;
                else if (s == "Chest")
                    return FreeFit::Data::MuscleGroup::Chest;
                else if (s == "Abs")
                    return FreeFit::Data::MuscleGroup::Abs;
                else if (s == "Arms")
                    return FreeFit::Data::MuscleGroup::Arms;
                else if (s == "Legs")
                    return FreeFit::Data::MuscleGroup::Legs;
                else
                    return FreeFit::Data::MuscleGroup::Error;
            }

            std::queue<std::shared_ptr<GUI::DownloadExerciseDemand>> demands;
            std::string path_to_db;
            std::shared_ptr<BaseXMLReader> r;
            std::shared_ptr<ExerciseWriter> w;
            std::shared_ptr<VideoDownload::YoutubeDL> yt;
        };

        class NewExerciseDemandHandlerValidator
        {
        public:
            NewExerciseDemandHandlerValidator(NewExerciseDemandHandler* t_h):h(t_h){}

            std::string getPathToDB(){return h->path_to_db;}
            std::shared_ptr<BaseXMLReader> getXMLReader(){return h->r;}
            std::shared_ptr<ExerciseWriter> getExerciseWriter(){return h->w;}
            std::shared_ptr<VideoDownload::YoutubeDL> getYoutubeDownloader(){return h->yt;}
            size_t getNumberOfDemands(){return h->demands.size();}            
        private:
            NewExerciseDemandHandler* h;
        };
    }
}
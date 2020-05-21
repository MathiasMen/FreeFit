#pragma once

#include <memory>
#include <queue>

#include "include/xmlreader.h"
#include "include/xmlwriter.h"
#include "include/xmlnode.h"
#include "include/exercise.h"
#include "include/downloadexercisedemand.h"
#include "include/datadefinitions.h"
#include "include/youtubedownloader.h"

namespace FreeFit
{
    namespace Data
    {
        class NewExerciseDemandHandlerValidator;

        class DownloadExerciseDemandHandler
        {
        friend NewExerciseDemandHandlerValidator;
        public:
            DownloadExerciseDemandHandler()
            {
                yt = std::make_shared<VideoDownload::YoutubeDL>();
                yt->setVideoFormat(VideoDownload::VideoType::MP4);
            }

            Exercise executeDemand(std::shared_ptr<GUI::DownloadExerciseDemand> d)
            {
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
                return e;
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
            std::shared_ptr<VideoDownload::YoutubeDL> yt;
        };

        class NewExerciseDemandHandlerValidator
        {
        public:
            NewExerciseDemandHandlerValidator(DownloadExerciseDemandHandler* t_h):h(t_h){}

            std::shared_ptr<VideoDownload::YoutubeDL> getYoutubeDownloader(){return h->yt;}
        private:
            DownloadExerciseDemandHandler* h;
        };
    }
}
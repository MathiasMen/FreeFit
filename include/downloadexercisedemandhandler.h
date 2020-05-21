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
            DownloadExerciseDemandHandler();
            Exercise executeDemand(std::shared_ptr<GUI::DownloadExerciseDemand> d);
        private:
            void downloadVideo(std::string url, std::string out_path, std::string start_time, std::string stop_time);
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
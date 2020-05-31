#include "include/downloadexercisedemandhandler.h"

namespace FreeFit
{
    namespace Data
    {
        DownloadExerciseDemandHandler::DownloadExerciseDemandHandler()
        {
            yt = std::make_shared<VideoDownload::YoutubeDL>();
            yt->setVideoFormat(VideoDownload::VideoType::MP4);
            cutter = new VideoDownload::ffmpegCutter();
        }

        Exercise DownloadExerciseDemandHandler::executeDemand(std::shared_ptr<GUI::DownloadExerciseDemand> d)
        {
            Exercise e;
            e.setName(d->name);
            for (auto a : d->muscle_areas)
                e.addTrainedMuscle(stringToMuscleGroup(a));
            std::string out_path_base = "/Users/mathias/Documents/programming_workspace/FreeFit/build/test/" + d->name + "_" + d->video_start_time + "_" + d->video_end_time;
            std::string out_path_video = out_path_base + ".mp4";
            std::string out_path_thumbnail = out_path_base + ".jpg";
            downloadVideo(d->video_url,out_path_video);
            out_path_video = cutVideo(out_path_video,d->video_start_time,d->video_end_time);
            e.setVideoURL(d->video_url);
            e.setVideoStartTime(d->video_start_time);
            e.setVideoEndTime(d->video_end_time);
            e.setVideoPath(out_path_video);
            e.setThumbnailPath(out_path_thumbnail);
            return e;
        }

        void DownloadExerciseDemandHandler::downloadVideo(std::string url, std::string out_path)
        {
                yt->download(url,out_path);
        }

        std::string DownloadExerciseDemandHandler::cutVideo(std::string in_path, std::string start_time, std::string stop_time)
        {
            unsigned int start = std::stoi(start_time);
            unsigned int end = std::stoi(stop_time);
            if (start > 0 && end > 0 && end > start)
                return cutter->cutVideo(in_path,start,end);
            else
                return in_path;
        }
    }
}
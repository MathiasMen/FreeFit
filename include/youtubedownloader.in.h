#pragma once 

#include <string>
#include <iostream>
#include "include/youtubeoptions.h"
#include "include/videocutter.h"

namespace VideoDownload
{

class YoutubeDownloader
{
public:
    virtual void download(std::string yt_url, std::string out_path) = 0;
    virtual void download(std::string yt_url, std::string out_path, int start_time, int end_time) = 0;

    void setVideoFormat(VideoType t){opt->setVideoFormat(t);}

protected:
    const std::string path_to_executable;
    YoutubeOptions* opt;
    VideoCutter* cutter;
};

class YoutubeDL : public YoutubeDownloader
{
public:
    YoutubeDL();

    void download(std::string yt_url, std::string out_path) override;
    void download(std::string yt_url, std::string out_path, int start_time, int end_time) override;

private:
    const std::string path_to_executable = "${CMAKE_SOURCE_DIR}/tools/YoutubeDL/youtube-dl";
};
}
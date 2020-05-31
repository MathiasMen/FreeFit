#pragma once 

#include <string>
#include <iostream>
#include "include/youtubeoptions.h"

namespace VideoDownload
{

class YoutubeDownloader
{
public:
    virtual void download(std::string yt_url, std::string out_path) = 0;

    void setVideoFormat(VideoType t){opt->setVideoFormat(t);}
protected:
    const std::string path_to_executable;
    YoutubeOptions* opt;
};

class YoutubeDL : public YoutubeDownloader
{
public:
    YoutubeDL();

    void download(std::string yt_url, std::string out_path) override;
private:
    const std::string path_to_executable = "${CMAKE_SOURCE_DIR}/tools/YoutubeDL/youtube-dl";
};
}
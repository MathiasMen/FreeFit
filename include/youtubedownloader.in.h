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
    virtual int getVideoLength(std::string yt_url) = 0;

    void setVideoFormat(VideoType t){opt->setVideoFormat(t);}
protected:
    std::string executeCommand(const char* cmd);

    const std::string path_to_executable;
    YoutubeOptions* opt;
};

class YoutubeDL : public YoutubeDownloader
{
public:
    YoutubeDL();

    void download(std::string yt_url, std::string out_path) override;
    int getVideoLength(std::string yt_url) override;

private:
    const std::string path_to_executable = "${CMAKE_SOURCE_DIR}/tools/YoutubeDL/youtube-dl";
};
}
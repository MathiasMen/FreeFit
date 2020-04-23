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
    virtual void download(std::string yt_url, std::string out_path, int start_time, int end_time) = 0;
    void setVideoFormat(VideoType t){opt->setVideoFormat(t);}
protected:
    const std::string path_to_executable;
    YoutubeOptions* opt;
};

class YoutubeDL : public YoutubeDownloader
{
public:
    YoutubeDL(){this->opt = new YoutubeDLOptions();}

    void download(std::string yt_url, std::string out_path) override
    {
        std::string dl_cmnd(this->path_to_executable +
                            opt->getOptionString() +
                            " -o \"" +
                            out_path +
                            "\" \"" +
                            yt_url +
                            "\""); 
        std::system(dl_cmnd.c_str());
    }

    void download(std::string yt_url, std::string out_path, int start_time, int end_time) override
    {
        download(yt_url,out_path);
        std::string new_name =  out_path.substr(0,out_path.find(".")) +
                                "_resized" +
                                out_path.substr(out_path.find("."),out_path.length() - 1);
        std::string cut_cmnd("ffmpeg -ss " + 
                            std::to_string(start_time) + 
                            " -i " +
                            out_path + 
                            " -to " +
                            std::to_string(end_time) + 
                            " -c copy " +
                            new_name);
        std::system(cut_cmnd.c_str());
    }
    private:
        const std::string path_to_executable = "${CMAKE_SOURCE_DIR}/tools/YoutubeDL/youtube-dl";
};
}
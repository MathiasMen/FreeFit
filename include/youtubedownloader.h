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
    YoutubeDL()
    {
        this->opt = new YoutubeDLOptions();
        this->cutter = new ffmpegCutter();
    }

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
        cutter->cutVideo(out_path,start_time,end_time);
    }
    private:
        const std::string path_to_executable = "/Users/mathias/Documents/programming_workspace/FreeFit/tools/YoutubeDL/youtube-dl";
};
}

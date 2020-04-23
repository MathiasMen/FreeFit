#pragma once 

#include <string>
#include <iostream>
#include "include/youtubeoptions.h"

class YoutubeDownloader
{
public:
    YoutubeDownloader(std::string t_path_to_executable) : path_to_executable(t_path_to_executable){}
    virtual void download(std::string yt_url, std::string out_path) = 0;
    virtual void download(std::string yt_url, std::string out_path, YoutubeOptions opt) = 0;
    virtual void download(std::string yt_url, std::string out_path, int start_time, int end_time, YoutubeOptions opt = YoutubeOptions()) = 0;
protected:
    std::string path_to_executable;
private:
    YoutubeDownloader();
};

class YoutubeDL : public YoutubeDownloader
{
public:
    YoutubeDL(std::string t_path_to_executable) : YoutubeDownloader(t_path_to_executable){}

    void download(std::string yt_url, std::string out_path) override
    {
        this->download(yt_url,out_path,YoutubeOptions());
    }

    void download(std::string yt_url, std::string out_path, YoutubeOptions opt) override
    {
        std::string dl_cmnd(this->path_to_executable +
                            opt.getOptions() +
                            " -f 'bestvideo[ext=mp4]+bestaudio[ext=m4a]' -o \"" +
                            out_path +
                            "\" \"" +
                            yt_url +
                            "\""); 
        std::system(dl_cmnd.c_str());
    }

    void download(std::string yt_url, std::string out_path, int start_time, int end_time, YoutubeOptions opt = YoutubeOptions()) override
    {
        download(yt_url,out_path, opt);
        std::string cut_cmnd("ffmpeg -ss " + 
                            std::to_string(start_time) + 
                            " -i " +
                            out_path + 
                            " -to " +
                            std::to_string(end_time) + 
                            " -c copy " +
                            out_path + "_resized.mp4");
        std::system(cut_cmnd.c_str());
    }
private:
    YoutubeDL();
};
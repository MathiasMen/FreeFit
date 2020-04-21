#include <string>
#include <iostream>
#include "include/youtubeoptions.h"

class YoutubeDownloader
{
public:
    YoutubeDownloader(std::string t_path_to_executable) : path_to_executable(t_path_to_executable){}
    virtual void download(std::string yt_url, std::string out_path) = 0;
    virtual void download(std::string yt_url, std::string out_path, YoutubeOptions opt) = 0;
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
        std::string command(this->path_to_executable +
                            opt.getOptions() +
                            " -o \"" +
                            out_path +
                            "/%(title)s.%(ext)s\" \"" +
                            yt_url +
                            "\""); 
        std::system(command.c_str());
    }
private:
    YoutubeDL();
};
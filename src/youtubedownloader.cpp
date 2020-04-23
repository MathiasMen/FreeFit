#include "include/youtubedownloader.h"

VideoDownload::YoutubeDL::YoutubeDL()
{
    this->opt = new YoutubeDLOptions();
    this->cutter = new ffmpegCutter();
}

void VideoDownload::YoutubeDL::download(std::string yt_url, std::string out_path)
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

void VideoDownload::YoutubeDL::download(std::string yt_url, std::string out_path, int start_time, int end_time)
{
    download(yt_url,out_path);
    cutter->cutVideo(out_path,start_time,end_time);
}


#include "include/youtubedownloader.h"

VideoDownload::YoutubeDL::YoutubeDL()
{
    this->opt = new YoutubeDLOptions();
}

void VideoDownload::YoutubeDL::download(std::string yt_url, std::string out_path)
{
    std::string dl_cmnd(this->path_to_executable +
                        opt->getOptionString() +
                        " --write-thumbnail -q --no-cache-dir -o \"" +
                        out_path +
                        "\" \"" +
                        yt_url +
                        "\""); 
    std::system(dl_cmnd.c_str());
}

#include "include/youtubedownloader.h"

std::string VideoDownload::YoutubeDownloader::executeCommand(const char* cmd)
{
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

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

int VideoDownload::YoutubeDL::getVideoLength(std::string yt_url)
{
    std::string dl_cmnd(this->path_to_executable +
                        " --get-duration \"" +
                        yt_url +
                        "\""); 
    return std::stoi(executeCommand(dl_cmnd.c_str()));
}
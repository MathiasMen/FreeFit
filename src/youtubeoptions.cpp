#include "include/youtubeoptions.h"

std::string VideoDownload::YoutubeDLOptions::getOptionString()
{
    std::string res = {};
    switch(v_t)
    {
    case VideoType::MP4 :
        res += " -f 'bestvideo[ext=mp4]+bestaudio[ext=m4a]'";
    case VideoType::MP3 :
        res += "";
    default:
        res += "";
    }

    for(std::string s : additional_options)
    res += std::string(" ") + s;

    return res;
}

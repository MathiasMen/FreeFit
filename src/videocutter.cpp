#include "include/videocutter.h"
std::string VideoDownload::ffmpegCutter::cutVideo(std::string out_path, int start_time, int end_time)
{
    int internal_length = end_time - start_time;
    std::string new_name =  out_path.substr(0,out_path.find(".")) +
                            "_resized" +
                            out_path.substr(out_path.find("."),out_path.length() - 1);
    std::string cut_cmnd("ffmpeg -y -hide_banner -loglevel panic -i " + 
                        out_path +
                        " -ss " +
                        std::to_string(start_time) + 
                        " -t " +
                        std::to_string(internal_length) +
                        " -async 1 " +
                        new_name);
    std::system(cut_cmnd.c_str());
    return new_name;
}

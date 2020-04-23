#include "include/videocutter.h"

void VideoDownload::ffmpegCutter::cutVideo(std::string out_path, int start_time, int end_time)
{
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

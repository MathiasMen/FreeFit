#pragma once

#include <string>

namespace VideoDownload
{
    class VideoCutter
    {
    public:
        virtual std::string cutVideo(std::string out_path, int start_time, int end_time) = 0;
    };

    class ffmpegCutter : public VideoCutter
    {
        std::string cutVideo(std::string out_path, int start_time, int end_time) override;
    };
}
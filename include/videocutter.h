#pragma once

#include <string>

namespace VideoDownload
{
    class VideoCutter
    {
    public:
        virtual void cutVideo(std::string out_path, int start_time, int end_time) = 0;
    };

    class ffmpegCutter : public VideoCutter
    {
        void cutVideo(std::string out_path, int start_time, int end_time) override;
    };
}
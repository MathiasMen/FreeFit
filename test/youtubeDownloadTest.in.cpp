#include <gtest/gtest.h>
#include <fstream>
#include "include/youtubeoptions.h"
#include "include/youtubedownloader.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(YoutubeDownload, Options)
{
    VideoDownload::YoutubeDLOptions opt;
    opt.addAdditionalOption("--help");
    opt.addAdditionalOption("--version");
    opt.addAdditionalOption("--someOtherArgument");

    ASSERT_EQ(opt.getOptionString()," --help --version --someOtherArgument");
}

TEST(YoutubeDownload, Download)
{
    std::string out_path = {"${CMAKE_BINARY_DIR}/test/Pushup.mp4"};

    VideoDownload::YoutubeDownloader* d = new VideoDownload::YoutubeDL();
    d->download("https://www.youtube.com/watch?v=-kwe1EOiWMY",out_path);

    std::ifstream f(out_path);
    ASSERT_TRUE(f.good());
}

TEST(YoutubeDownload, DownloadAndResize)
{
    std::string out_path = {"${CMAKE_BINARY_DIR}/test/Pushup.mp4"};

    VideoDownload::YoutubeDownloader* d = new VideoDownload::YoutubeDL();
    VideoDownload::VideoCutter* cutter = new VideoDownload::ffmpegCutter();

    d->setVideoFormat(VideoDownload::VideoType::MP4);
    d->download("https://www.youtube.com/watch?v=-kwe1EOiWMY",out_path);
    
    std::ifstream f1(out_path);
    ASSERT_TRUE(f1.good());
    
    out_path = cutter->cutVideo(out_path,30,90);
    
    std::ifstream f2("${CMAKE_BINARY_DIR}/test/Pushup_resized.mp4");
    ASSERT_TRUE(f2.good());
}

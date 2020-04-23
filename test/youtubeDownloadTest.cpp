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

    EXPECT_EQ(opt.getOptionString()," --help --version --someOtherArgument");
}

TEST(YoutubeDownload, Download)
{
    std::string out_path = {"/Users/mathias/Documents/programming_workspace/FreeFit/build/test/FlorenceAndTheMachine"};
    VideoDownload::YoutubeDownloader* d = new VideoDownload::YoutubeDL();
    d->download("https://www.youtube.com/watch?v=nyiNSFp2uf0",out_path);
    std::ifstream f(out_path+".mkv");
    ASSERT_TRUE(f.good());
}

TEST(YoutubeDownload, DownloadAndResize)
{
    std::string out_path = {"/Users/mathias/Documents/programming_workspace/FreeFit/build/test/LouReed.mp4"};
    VideoDownload::YoutubeDownloader* d = new VideoDownload::YoutubeDL();
    d->setVideoFormat(VideoDownload::VideoType::MP4);
    d->download("https://www.youtube.com/watch?v=oG6fayQBm9w",out_path, 30, 90);
    std::ifstream f("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/LouReed_resized.mp4");
    ASSERT_TRUE(f.good());
}

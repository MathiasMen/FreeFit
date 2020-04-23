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
    YoutubeOptions opt;
    opt.addOptions("--help");
    opt.addOptions("--version");
    opt.addOptions("--someOtherArgument");

    EXPECT_EQ(opt.getOptions()," --help --version --someOtherArgument");
}

TEST(YoutubeDownload, Download)
{
    std::string out_path = {"${CMAKE_BINARY_DIR}/test/FlorenceAndTheMachine.mp4"};
    YoutubeDownloader* d = new YoutubeDL("${CMAKE_SOURCE_DIR}/tools/YoutubeDL/youtube-dl");
    d->download("https://www.youtube.com/watch?v=nyiNSFp2uf0",out_path);
    std::ifstream f(out_path);
    ASSERT_TRUE(f.good());
}

TEST(YoutubeDownload, DownloadAndResize)
{
    std::string out_path = {"${CMAKE_BINARY_DIR}/test/LouReed.mp4"};
    YoutubeDownloader* d = new YoutubeDL("${CMAKE_SOURCE_DIR}/tools/YoutubeDL/youtube-dl");
    d->download("https://www.youtube.com/watch?v=oG6fayQBm9w",out_path, 30, 90);
    std::ifstream f(out_path);
    ASSERT_TRUE(f.good());
}

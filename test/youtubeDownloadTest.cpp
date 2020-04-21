#include <gtest/gtest.h>
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

TEST(YoutubeDownload, Downloader)
{
    YoutubeDownloader* d = new YoutubeDL("/Users/mathias/Documents/programming_workspace/FreeFit/tools/YoutubeDL/youtube-dl");
    d->download("https://www.youtube.com/watch?v=nyiNSFp2uf0","/Users/mathias/Documents/programming_workspace/FreeFit/build");
}
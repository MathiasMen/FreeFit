#include <gtest/gtest.h>
#include "include/youtubeoptions.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(YoutubeDownload, Options_1)
{
    YoutubeOptions opt;
    opt.addOptions("--help");
    opt.addOptions("--version");
    opt.addOptions("--someOtherArgument");

    EXPECT_EQ(opt.getOptions()," --help --version --someOtherArgument");
}
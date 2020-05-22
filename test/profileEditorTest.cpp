#include <gtest/gtest.h>

#include <QApplication>

#include "include/profileeditor.h"

int my_argc;
char** my_argv;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    my_argc = argc;
    my_argv = argv;
    return RUN_ALL_TESTS();
}

class ProfileEditor : public ::testing::Test
{
    protected:
        std::string profile_path = "/Users/mathias/Documents/programming_workspace/FreeFit/build/test/test_profile.xml";

    virtual void SetUp()
    {
    }
};

TEST_F(ProfileEditor,LaunchEditor)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ProfileEditor* d = new FreeFit::GUI::ProfileEditor(profile_path);
    d->exec();
}

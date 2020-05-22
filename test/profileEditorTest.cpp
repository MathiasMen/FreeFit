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
        std::string profile_path = "/Users/mathias/Documents/programming_workspace/FreeFit/test/ProfileEditorReadInTest.xml";
        std::string exercises_path = "/Users/mathias/Documents/programming_workspace/FreeFit/build/test/";
        std::string profile_name = "Mathias";
};

TEST_F(ProfileEditor,LaunchEditor)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ProfileEditor* d = new FreeFit::GUI::ProfileEditor(profile_path);
}

TEST_F(ProfileEditor,ReadXML)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ProfileEditor* d = new FreeFit::GUI::ProfileEditor(profile_path);
    ASSERT_EQ(d->getExercisesPath(),exercises_path);
    ASSERT_EQ(d->getName(),profile_name);
}

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
    d->exec();
}

TEST_F(ProfileEditor,ReadXML)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ProfileEditor* d = new FreeFit::GUI::ProfileEditor(profile_path);
    ASSERT_EQ(d->getExercisesPath(),exercises_path);
    ASSERT_EQ(d->getName(),profile_name);
}

TEST_F(ProfileEditor,WriteXML)
{
    std::string out_path = "/Users/mathias/Documents/programming_workspace/FreeFit/build/test/ProfileEditorWriteXML.xml";
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ProfileEditor* d = new FreeFit::GUI::ProfileEditor(profile_path);
    d->setXMLOutPath(out_path);
    d->setName("MathiasMentler");
    d->accept();

    std::string expected_profile_xml = 
    "<PROFILE>\n"
    "  <NAME>\n"
    "    MathiasMentler\n"
    "  </NAME>\n"
    "  <PICTUREPATH>\n"
    "    DummyPicturePath\n"
    "  </PICTUREPATH>\n"
    "  <DATELASTWORKOUT>\n"
    "    24.03.1994\n"
    "  </DATELASTWORKOUT>\n"
    "  <PATHEXDB>\n"
    "    /Users/mathias/Documents/programming_workspace/FreeFit/build/test/\n"
    "  </PATHEXDB>\n"
    "  <PERFFACTOR>\n"
    "    10,000000\n"
    "  </PERFFACTOR>\n"
    "</PROFILE>\n";

    std::ifstream f(out_path);
    std::stringstream ss;
    ss << f.rdbuf();

    ASSERT_EQ(ss.str(),expected_profile_xml);
}

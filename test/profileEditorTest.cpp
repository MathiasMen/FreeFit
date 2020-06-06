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
};

TEST_F(ProfileEditor,ReadXML)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ProfileEditor* d = new FreeFit::GUI::ProfileEditor(profile_path);
    FreeFit::GUI::ProfileEditorValidator* v = new FreeFit::GUI::ProfileEditorValidator(d);
    ASSERT_EQ(v->getNumberOfLoadedProfiles(),2);
    ASSERT_EQ(v->getProfile(0).getName(),"Mathias");
    ASSERT_EQ(v->getProfile(1).getName(),"Constanze");
}

TEST_F(ProfileEditor,WriteXML)
{
    std::string out_path = "/Users/mathias/Documents/programming_workspace/FreeFit/build/test/ProfileEditorWriteXML.xml";
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ProfileEditor* d = new FreeFit::GUI::ProfileEditor(profile_path);
    FreeFit::GUI::ProfileEditorValidator* v = new FreeFit::GUI::ProfileEditorValidator(d);
    v->setXMLOutPath(out_path);
    v->setName("Mathias2");
    d->accept();

    std::string expected_xml = 
    "<PROFILES>\n"
    "  <PROFILE>\n"
    "    <NAME>\n"
    "      Mathias2\n"
    "    </NAME>\n"
    "    <PICTUREPATH>\n"
    "      DummyPicturePath\n"
    "    </PICTUREPATH>\n"
    "    <DATELASTWORKOUT>\n"
    "      23.05.1989\n"
    "    </DATELASTWORKOUT>\n"
    "    <PATHEXDB>\n"
    "      /Users/mathias/Documents/programming_workspace/FreeFit/test/ReadXMLAndPopulateExerciseList.xml\n"
    "    </PATHEXDB>\n"
    "    <PERFFACTOR>\n"
    "      10,000000\n"
    "    </PERFFACTOR>\n"
    "  </PROFILE>\n"
    "  <PROFILE>\n"
    "    <NAME>\n"
    "      Constanze\n"
    "    </NAME>\n"
    "    <PICTUREPATH>\n"
    "      DummyPicturePath\n"
    "    </PICTUREPATH>\n"
    "    <DATELASTWORKOUT>\n"
    "      24.03.1994\n"
    "    </DATELASTWORKOUT>\n"
    "    <PATHEXDB>\n"
    "      /Users/mathias/Documents/programming_workspace/FreeFit/build/test/\n"
    "    </PATHEXDB>\n"
    "    <PERFFACTOR>\n"
    "      11,000000\n"
    "    </PERFFACTOR>\n"
    "  </PROFILE>\n"
    "</PROFILES>\n";

    std::ifstream f(out_path);
    std::stringstream ss;
    ss << f.rdbuf();

    ASSERT_EQ(ss.str(),expected_xml);
}

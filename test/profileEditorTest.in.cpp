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
        std::string profile_path = "${CMAKE_SOURCE_DIR}/test/input/ProfileEditorReadInTest.xml";
};

TEST_F(ProfileEditor,ReadXML)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ProfileEditor* d = new FreeFit::GUI::ProfileEditor(profile_path);
    FreeFit::GUI::ProfileEditorValidator* v = new FreeFit::GUI::ProfileEditorValidator(d);
    d->open();
    ASSERT_EQ(v->getNumberOfLoadedProfiles(),2);
    ASSERT_EQ(v->getProfileData(0).getName(),"Mathias");
    ASSERT_EQ(v->getProfileData(1).getName(),"Constanze");
    ASSERT_EQ(v->getProfileData(0).getColor(),"#00ff00");
    ASSERT_EQ(v->getProfileData(1).getColor(),"#0000ff");
    ASSERT_EQ(v->getProfileData(0).getPathToExerciseDB(),"${CMAKE_SOURCE_DIR}/test/input/ReadXMLAndPopulateExerciseList.xml");
    ASSERT_EQ(v->getProfileData(1).getPathToExerciseDB(),"${CMAKE_SOURCE_DIR}/test/input/ReadXMLAndPopulateExerciseList2.xml");
    d->resize(1024,640);
    d->reject();
}

TEST_F(ProfileEditor,EditSelection)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ProfileEditor* d = new FreeFit::GUI::ProfileEditor(profile_path);
    FreeFit::GUI::ProfileEditorValidator* v = new FreeFit::GUI::ProfileEditorValidator(d);
    d->open();
    ASSERT_EQ(v->getNumberOfLoadedProfiles(),2);
    v->setCurrentName("MathiasTest");
    v->setCurrentColor("#00ff00");
    ASSERT_EQ(v->getCurrentName(),"MathiasTest");
    ASSERT_EQ(v->getCurrentColor(),"#00ff00");
    ASSERT_TRUE(v->getNextButtonCSS().find("background-color:#00ff00") != std::string::npos);
    ASSERT_TRUE(v->getNextButtonCSS().find("color:#ffffff") != std::string::npos);
    ASSERT_TRUE(v->getSkipButtonCSS().find("background-color:#ffffff") != std::string::npos);
    ASSERT_TRUE(v->getSkipButtonCSS().find("color:#00ff00") != std::string::npos);
    ASSERT_EQ(v->getProfileData(0).getName(),"MathiasTest");
    ASSERT_EQ(v->getProfileData(1).getName(),"Constanze");
    d->reject();
}

TEST_F(ProfileEditor,ChangeSelection)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ProfileEditor* d = new FreeFit::GUI::ProfileEditor(profile_path);
    FreeFit::GUI::ProfileEditorValidator* v = new FreeFit::GUI::ProfileEditorValidator(d);
    d->open();
    ASSERT_EQ(v->getNumberOfLoadedProfiles(),2);
    ASSERT_EQ(v->getProfileData(0).getName(),"Mathias");
    ASSERT_EQ(v->getProfileData(1).getName(),"Constanze");
    ASSERT_EQ(v->getProfileData(0).getPathToExerciseDB(),"${CMAKE_SOURCE_DIR}/test/input/ReadXMLAndPopulateExerciseList.xml");
    ASSERT_EQ(v->getProfileData(1).getPathToExerciseDB(),"${CMAKE_SOURCE_DIR}/test/input/ReadXMLAndPopulateExerciseList2.xml");
    ASSERT_EQ(v->getCurrentName(),"Mathias");
    ASSERT_EQ(v->getCurrentColor(),"#00ff00");
    v->selectProfile(1);
    ASSERT_EQ(v->getCurrentName(),"Constanze");
    ASSERT_EQ(v->getCurrentColor(),"#0000ff");
    v->selectProfile(0);
    ASSERT_EQ(v->getCurrentName(),"Mathias");
    ASSERT_EQ(v->getCurrentColor(),"#00ff00");
    d->reject();
}

TEST_F(ProfileEditor,ChangeSelectionAndEdit)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ProfileEditor* d = new FreeFit::GUI::ProfileEditor(profile_path);
    FreeFit::GUI::ProfileEditorValidator* v = new FreeFit::GUI::ProfileEditorValidator(d);
    d->open();
    ASSERT_EQ(v->getNumberOfLoadedProfiles(),2);
    v->selectProfile(1);
    v->setCurrentName("ConstanzeTest");
    v->selectProfile(0);
    v->selectProfile(1);
    ASSERT_EQ(v->getCurrentName(),"ConstanzeTest");
    d->reject();
}

TEST_F(ProfileEditor,CreateProfilesAndWriteXML)
{
    std::string out_path = "${CMAKE_SOURCE_DIR}/build/test/ProfileEditorCreateProfileXML.xml";
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ProfileEditor* d = new FreeFit::GUI::ProfileEditor("${CMAKE_SOURCE_DIR}/test/input/EmptyProfiles.xml");
    FreeFit::GUI::ProfileEditorValidator* v = new FreeFit::GUI::ProfileEditorValidator(d);
    v->changeOutputPath(out_path);
    ASSERT_EQ(v->getNumberOfLoadedProfiles(),1);
    v->setCurrentName("Mathias");
    v->setCurrentColor("#00ff00");
    ASSERT_EQ(v->getCurrentName(),"Mathias");
    ASSERT_EQ(v->getCurrentColor(),"#00ff00");
    ASSERT_EQ(v->getNumberOfLoadedProfiles(),2);
    v->selectProfile(1);
    ASSERT_EQ(v->getCurrentName(),FreeFit::Data::Profile::buildNewProfileProfile().getName());
    ASSERT_EQ(v->getCurrentColor(),FreeFit::Data::Profile::buildNewProfileProfile().getColor());
    v->setCurrentName("Constanze");
    v->setCurrentColor("#0000ff");
    ASSERT_EQ(v->getNumberOfLoadedProfiles(),3);
    d->accept();
    ASSERT_EQ(v->getCurrentExercisesPath(),"${CMAKE_BINARY_DIR}/Constanze.xml");

    std::string expected_xml = 
    "<PROFILES>\n"
    "  <PROFILE>\n"
    "    <NAME>\n"
    "      Mathias\n"
    "    </NAME>\n"
    "    <COLOR>\n"
    "      #00ff00\n"
    "    </COLOR>\n"
    "    <PICTUREPATH>\n"
    "    </PICTUREPATH>\n"
    "    <DATELASTWORKOUT>\n"
    "    </DATELASTWORKOUT>\n"
    "    <PATHEXDB>\n"
    "      ${CMAKE_BINARY_DIR}/Mathias.xml\n"
    "    </PATHEXDB>\n"
    "    <PERFFACTOR>\n"
    "      1,000000\n"
    "    </PERFFACTOR>\n"
    "  </PROFILE>\n"
    "  <PROFILE>\n"
    "    <NAME>\n"
    "      Constanze\n"
    "    </NAME>\n"
    "    <COLOR>\n"
    "      #0000ff\n"
    "    </COLOR>\n"
    "    <PICTUREPATH>\n"
    "    </PICTUREPATH>\n"
    "    <DATELASTWORKOUT>\n"
    "    </DATELASTWORKOUT>\n"
    "    <PATHEXDB>\n"
    "      ${CMAKE_BINARY_DIR}/Constanze.xml\n"
    "    </PATHEXDB>\n"
    "    <PERFFACTOR>\n"
    "      1,000000\n"
    "    </PERFFACTOR>\n"
    "  </PROFILE>\n"
    "</PROFILES>\n";

    std::ifstream f(out_path);
    std::stringstream ss;
    ss << f.rdbuf();

    ASSERT_EQ(ss.str(),expected_xml);
}

// TEST_F(ProfileEditor,CreateProfileCheckPathsGeneratedCorrectly)

TEST_F(ProfileEditor,WriteXML)
{
    std::string out_path = "${CMAKE_SOURCE_DIR}/build/test/ProfileEditorWriteXML.xml";
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ProfileEditor* d = new FreeFit::GUI::ProfileEditor(profile_path);
    FreeFit::GUI::ProfileEditorValidator* v = new FreeFit::GUI::ProfileEditorValidator(d);
    v->changeOutputPath(out_path);
    v->setCurrentName("Mathias2");
    v->setCurrentColor("#008000");
    d->accept();

    std::string expected_xml = 
    "<PROFILES>\n"
    "  <PROFILE>\n"
    "    <NAME>\n"
    "      Mathias2\n"
    "    </NAME>\n"
    "    <COLOR>\n"
    "      #008000\n"
    "    </COLOR>\n"
    "    <PICTUREPATH>\n"
    "      DummyPicturePath\n"
    "    </PICTUREPATH>\n"
    "    <DATELASTWORKOUT>\n"
    "      23.05.1989\n"
    "    </DATELASTWORKOUT>\n"
    "    <PATHEXDB>\n"
    "      ${CMAKE_SOURCE_DIR}/test/input/ReadXMLAndPopulateExerciseList.xml\n"
    "    </PATHEXDB>\n"
    "    <PERFFACTOR>\n"
    "      10,000000\n"
    "    </PERFFACTOR>\n"
    "  </PROFILE>\n"
    "  <PROFILE>\n"
    "    <NAME>\n"
    "      Constanze\n"
    "    </NAME>\n"
    "    <COLOR>\n"
    "      #0000ff\n"
    "    </COLOR>\n"
    "    <PICTUREPATH>\n"
    "      DummyPicturePath\n"
    "    </PICTUREPATH>\n"
    "    <DATELASTWORKOUT>\n"
    "      24.03.1994\n"
    "    </DATELASTWORKOUT>\n"
    "    <PATHEXDB>\n"
    "      ${CMAKE_SOURCE_DIR}/test/input/ReadXMLAndPopulateExerciseList2.xml\n"
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

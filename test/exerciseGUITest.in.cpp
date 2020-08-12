#include <gtest/gtest.h>

#include <memory>
#include <cstdio>

#include <QDialog>

#include "include/exerciseeditor.h"
#include "include/downloadexercisedemand.h"
#include "include/profile.h"
#include "include/exercise.h"
#include "include/downloadexercisedemandhandler.h"

int my_argc;
char** my_argv;
QApplication* app;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    my_argc = argc;
    my_argv = argv;
    app = new QApplication(my_argc,my_argv);
    return RUN_ALL_TESTS();
}

TEST(DownloadExerciseDemand, Init)
{
    std::string name = "Name", video_url = "TestURL", video_start_time = "2", video_end_time = "5";

    FreeFit::GUI::DownloadExerciseDemand d;
    d.name = name;
    d.video_url = video_url;
    d.video_start_time = video_start_time;
    d.video_end_time = video_end_time;
    d.muscle_areas.push_back("Shoulders");
    d.muscle_areas.push_back("Chest");

    ASSERT_EQ(d.muscle_areas.size(),2);
}

TEST(DownloadExerciseDemandHandler, Init)
{
    FreeFit::Data::DownloadExerciseDemandHandler h;

    FreeFit::Data::NewExerciseDemandHandlerValidator v(&h);

    ASSERT_NE(v.getYoutubeDownloader(),nullptr);
}

TEST(DownloadExerciseDemandHandler, ExecuteDemand)
{
    FreeFit::Data::DownloadExerciseDemandHandler h;
    FreeFit::Data::NewExerciseDemandHandlerValidator v(&h);
    std::shared_ptr<FreeFit::GUI::DownloadExerciseDemand> d = std::make_shared<FreeFit::GUI::DownloadExerciseDemand>();
    d->name = "Pushup";
    d->video_url = "https://www.youtube.com/watch?v=IODxDxX7oi4&t=35s";
    d->video_start_time = "35";
    d->video_end_time = "40";
    d->muscle_areas.push_back("Shoulder");
    d->muscle_areas.push_back("Chest");
    FreeFit::Data::Exercise e = h.executeDemand(d);
    std::string out_path = "${CMAKE_SOURCE_DIR}/build/test/Pushup_35_40_resized.mp4";
    std::ifstream f(out_path);
    ASSERT_EQ(e.getVideoPath(),out_path);
    ASSERT_TRUE(f.is_open());
}

class ExerciseEditor : public ::testing::Test
{
    protected:
    FreeFit::Data::Profile p;

    virtual void SetUp()
    {
        p.setName("John Doe");
        p.setDateLastWorkout("31.12.2000");
        p.setPathToExerciseDB("${CMAKE_SOURCE_DIR}/build/test/exercises.xml");
        p.setPerformanceFactor(1.0);
        p.setPicturePath("none");
    }
};

TEST_F(ExerciseEditor,LaunchEditor)
{
    p.setPathToExerciseDB("${CMAKE_SOURCE_DIR}/test/input/LaunchEditor.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    e->open();
    v.pushAddButton();
    ASSERT_EQ(v.getNumberOfExercises(),1);
    e->accept();
}

TEST_F(ExerciseEditor,AddButton)
{
    p.setPathToExerciseDB("${CMAKE_SOURCE_DIR}/test/input/LaunchEditor.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    e->open();
    v.pushAddButton();
    v.pushAddButton();
    ASSERT_EQ(v.getNumberOfExercisesToDownload(),2);
    ASSERT_EQ(v.getNumberOfExercises(),1);
    e->accept();
}

TEST_F(ExerciseEditor,DeleteButton)
{
    p.setPathToExerciseDB("${CMAKE_SOURCE_DIR}/test/input/LaunchEditor.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.pushAddButton();
    v.pushAddButton();
    v.pushFirstDeleteButtonOldExercises();
    v.pushFirstDeleteButtonNewExercises();
    ASSERT_EQ(v.getNumberOfExercisesToDownload(),1);
    ASSERT_EQ(v.getNumberOfExercises(),0);
}

TEST_F(ExerciseEditor,ExerciseDemand)
{
    std::string ex_name = "Pushup";
    std::string ex_url = "https://www.youtube.com/watch?v=IODxDxX7oi4";
    
    p.setPathToExerciseDB("${CMAKE_SOURCE_DIR}/test/input/ExerciseDemand.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.setOutPath("${CMAKE_SOURCE_DIR}/build/test/ExerciseDemand_out.xml");
    v.pushAddButton();
    v.setFirstNewExerciseNameText(ex_name);
    v.setFirstNewExerciseURLText(ex_url);
    v.setFirstNewExerciseMuscleArea(0);
    v.setFirstNewExerciseMuscleArea(2);

    e->open();
    std::shared_ptr<FreeFit::GUI::DownloadExerciseDemand> d = v.getFirstExerciseDemand();

    auto it = d->muscle_areas.begin();
    ASSERT_EQ(d->name,ex_name);
    ASSERT_EQ(d->video_url,ex_url);
    ASSERT_EQ(*it,"Shoulder");
    ASSERT_EQ(*(++it),"Chest");
    e->accept();
}

TEST_F(ExerciseEditor,ValidateFunctionTrue)
{
    std::string ex_name = "Pushup";
    std::string ex_url = "https://www.youtube.com/watch?v=IODxDxX7oi4";
    
    p.setPathToExerciseDB("${CMAKE_SOURCE_DIR}/test/input/LaunchEditor.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.setOutPath("${CMAKE_SOURCE_DIR}/build/test/ValidateFunctionTrue.xml");
    e->open();
    
    v.pushAddButton();
    v.setFirstNewExerciseNameText(ex_name);
    v.setFirstNewExerciseURLText(ex_url);

    ASSERT_TRUE(v.isFirstNewExerciseNameValid());
    ASSERT_TRUE(v.isFirstNewExerciseURLValid());
    e->accept();
}

TEST_F(ExerciseEditor,ValidateFunctionNameWrong)
{
    p.setPathToExerciseDB("${CMAKE_SOURCE_DIR}/test/input/LaunchEditor.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.setOutPath("${CMAKE_SOURCE_DIR}/build/test/ValidateFunctionNameWrong.xml");
    e->open();

    v.pushAddButton();

    v.setFirstNewExerciseNameText("Pushup!");
    ASSERT_FALSE(v.isFirstNewExerciseNameValid());

    v.setFirstNewExerciseNameText("Pushupß");
    ASSERT_FALSE(v.isFirstNewExerciseNameValid());
    
    std::string dump;
    for (unsigned int i = 1; i <= 257;i++)
        dump += "a";
    v.setFirstNewExerciseNameText(dump);
    ASSERT_FALSE(v.isFirstNewExerciseNameValid());
    e->accept();
}

TEST_F(ExerciseEditor,ValidateFunctionURLWrong)
{
    p.setPathToExerciseDB("${CMAKE_SOURCE_DIR}/test/input/LaunchEditor.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.setOutPath("${CMAKE_SOURCE_DIR}/build/test/ValidateFunctionURLWrong.xml");
    e->open();

    v.pushAddButton();

    v.setFirstNewExerciseURLText("https://www.NOTyoutube.com/watch?v=BxIUwbb1Nzg");
    ASSERT_FALSE(v.isFirstNewExerciseURLValid());

    v.setFirstNewExerciseURLText("http://www.NOTyoutube.com/watch?v=BxIUwbb1Nzg");
    ASSERT_FALSE(v.isFirstNewExerciseURLValid());

    v.setFirstNewExerciseURLText("https://www.youtube.com/embed/nyiNSFp2uf0");
    ASSERT_FALSE(v.isFirstNewExerciseURLValid());
    e->accept();
}

TEST_F(ExerciseEditor,NewExercisesEditText)
{
    p.setPathToExerciseDB("${CMAKE_SOURCE_DIR}/test/input/LaunchEditor.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    e->open();
    v.pushAddButton();
    v.pushAddButton();

    std::string first_exercise_name = "Pushup";
    std::string second_exercise_name = "Pushup";
    std::string first_exercise_url = "http://www.youtube.com/watch?v=BxIUwbb1Nzg";
    std::string second_exercise_url = "http://www.youtube.com/watch?v=BxIUwbb1Nzh";

    v.setFirstNewExerciseNameText(first_exercise_name);
    v.setLastNewExerciseNameText(second_exercise_name);
    v.setFirstNewExerciseURLText(first_exercise_url);
    v.setLastNewExerciseURLText(second_exercise_url);

    ASSERT_EQ(v.getFirstNewExerciseNameText(),first_exercise_name);
    ASSERT_EQ(v.getLastNewExerciseNameText(),second_exercise_name);
    ASSERT_EQ(v.getFirstNewExerciseURLText(),first_exercise_url);
    ASSERT_EQ(v.getLastNewExerciseURLText(),second_exercise_url);

    e->accept();
}

TEST_F(ExerciseEditor,NewExercisesEditTags)
{
    p.setPathToExerciseDB("${CMAKE_SOURCE_DIR}/test/input/LaunchEditor.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    e->open();
    v.pushAddButton();
    v.pushAddButton();

    v.setFirstNewExerciseMuscleArea(0);
    v.setFirstNewExerciseMuscleArea(2);
    v.setLastNewExerciseMuscleArea(1);
    v.setLastNewExerciseMuscleArea(3);

    ASSERT_TRUE(v.isFirstNewExerciseMuscleAreaSelected(0));
    ASSERT_TRUE(v.isFirstNewExerciseMuscleAreaSelected(2));
    ASSERT_TRUE(v.isLastNewExerciseMuscleAreaSelected(1));
    ASSERT_TRUE(v.isLastNewExerciseMuscleAreaSelected(3));

    v.setFirstNewExerciseMuscleArea(0);
    v.setFirstNewExerciseMuscleArea(2);
    v.setLastNewExerciseMuscleArea(1);
    v.setLastNewExerciseMuscleArea(3);

    ASSERT_FALSE(v.isFirstNewExerciseMuscleAreaSelected(0));
    ASSERT_FALSE(v.isFirstNewExerciseMuscleAreaSelected(2));
    ASSERT_FALSE(v.isLastNewExerciseMuscleAreaSelected(1));
    ASSERT_FALSE(v.isLastNewExerciseMuscleAreaSelected(3));
    e->accept();
}

TEST_F(ExerciseEditor,DownloadClicked)
{
    std::string ex_name = "Pushup";
    std::string ex_url = "https://www.youtube.com/watch?v=IODxDxX7oi4";
    
    p.setPathToExerciseDB("${CMAKE_SOURCE_DIR}/test/input/LaunchEditor.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.setOutPath("${CMAKE_SOURCE_DIR}/test/input/DownloadClicked.xml");
    v.pushAddButton();
    v.setFirstNewExerciseNameText(ex_name);
    v.setFirstNewExerciseURLText(ex_url);
    v.setFirstNewExerciseMuscleArea(0);
    v.setFirstNewExerciseMuscleArea(2);

    e->open();
    v.connectToDownloadSignalsOfItems();
    v.pushDownloadAllButton();

    FreeFit::GUI::DownloadExerciseDemand d = v.getLastDemand();

    auto it = d.muscle_areas.begin();
    ASSERT_EQ(d.name,ex_name);
    ASSERT_EQ(d.video_url,ex_url);
    ASSERT_EQ(*it,"Shoulder");
    ASSERT_EQ(*(++it),"Chest");

    std::ifstream f("${CMAKE_SOURCE_DIR}/build/test/Pushup_00:02_00:03.mp4");
    ASSERT_TRUE(f.is_open());
    e->accept();
}

TEST_F(ExerciseEditor,ReadXMLAndPopulateExerciseList)
{
    p.setPathToExerciseDB("${CMAKE_SOURCE_DIR}/test/input/ReadXMLAndPopulateExerciseList.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.setOutPath("${CMAKE_SOURCE_DIR}/build/test/ReadXMLAndPopulateExerciseListOut.xml");
    e->open();
    ASSERT_EQ(v.getNumberOfExercises(),2);
    e->accept();
}

TEST_F(ExerciseEditor,XMLOutput)
{
    std::string ex_name = "Situp";
    std::string ex_url = "https://www.youtube.com/watch?v=O-3uPB3fgys";
    
    std::string out_path = "${CMAKE_SOURCE_DIR}/build/test/XMLOutput.xml";
    const int result = std::remove(out_path.c_str());
    p.setPathToExerciseDB(out_path);
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);

    FreeFit::GUI::ExerciseEditorValidator v(e);
    out_path = "${CMAKE_SOURCE_DIR}/build/test/XMLOutputOut.xml";
    v.setOutPath(out_path);
    v.setFirstNewExerciseNameText(ex_name);
    v.setFirstNewExerciseURLText(ex_url);
    v.setFirstNewExerciseMuscleArea(0);
    v.setFirstNewExerciseMuscleArea(2);

    v.pushDownloadAllButton();

    e->accept();

    std::string expected_exercises_xml = 
    "<EXERCISES>\n"
    "  <EXERCISE>\n"
    "    <NAME>\n"
    "      Situp\n"
    "    </NAME>\n"
    "    <VIDEOURL>\n"
    "      https://www.youtube.com/watch?v=O-3uPB3fgys\n"
    "    </VIDEOURL>\n"
    "    <VIDEOPATH>\n"
    "      ${CMAKE_SOURCE_DIR}/build/test/Situp_50_53_resized.mp4\n"
    "    </VIDEOPATH>\n"
    "    <THUMBNAILPATH>\n"
    "      ${CMAKE_SOURCE_DIR}/build/test/Situp_50_53.jpg\n"
    "    </THUMBNAILPATH>\n"
    "    <VIDEOSTARTTIME>\n"
    "      50\n"
    "    </VIDEOSTARTTIME>\n"
    "    <VIDEOENDTIME>\n"
    "      53\n"
    "    </VIDEOENDTIME>\n"
    "    <TRAINEDAREAS>\n"
    "      <AREA>\n"
    "        0\n"
    "      </AREA>\n"
    "      <AREA>\n"
    "        2\n"
    "      </AREA>\n"
    "    </TRAINEDAREAS>\n"
    "  </EXERCISE>\n"
    "</EXERCISES>\n";

    std::ifstream f1("${CMAKE_SOURCE_DIR}/build/test/Situp_50_53_resized.mp4");
    std::ifstream f2("${CMAKE_SOURCE_DIR}/build/test/Situp_50_53.mp4");
    std::ifstream f3(out_path);
    std::stringstream ss;
    ss << f3.rdbuf();

    ASSERT_TRUE(f1.is_open());
    ASSERT_TRUE(f2.is_open());
    ASSERT_EQ(ss.str(),expected_exercises_xml);
}

TEST_F(ExerciseEditor,AddExerciseToExistingXML)
{
    std::string out_path = "${CMAKE_SOURCE_DIR}/build/test/AddExerciseToExistingXML.xml";
    std::ofstream out_file(out_path,std::ios::trunc);
    std::string out_content = 
    "<EXERCISES>\n"
    "  <EXERCISE>\n"
    "    <NAME>\n"
    "      Situp\n"
    "    </NAME>\n"
    "    <VIDEOURL>\n"
    "      https://www.youtube.com/watch?v=O-3uPB3fgys\n"
    "    </VIDEOURL>\n"
    "    <VIDEOPATH>\n"
    "      ${CMAKE_SOURCE_DIR}/build/test/Situp_50_53_resized.mp4\n"
    "    </VIDEOPATH>\n"
    "    <THUMBNAILPATH>\n"
    "      ${CMAKE_SOURCE_DIR}/build/test/Situp_50_53.jpg\n"
    "    </THUMBNAILPATH>\n"
    "    <VIDEOSTARTTIME>\n"
    "      50\n"
    "    </VIDEOSTARTTIME>\n"
    "    <VIDEOENDTIME>\n"
    "      53\n"
    "    </VIDEOENDTIME>\n"
    "    <TRAINEDAREAS>\n"
    "      <AREA>\n"
    "        0\n"
    "      </AREA>\n"
    "      <AREA>\n"
    "        2\n"
    "      </AREA>\n"
    "    </TRAINEDAREAS>\n"
    "  </EXERCISE>\n"
    "</EXERCISES>\n";
    out_file << out_content;
    out_file.close();

    p.setPathToExerciseDB(out_path);
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);

    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.pushAddButton();
    v.setLastNewExerciseNameText("SomethingElse");
    v.setLastNewExerciseURLText("https://www.youtube.com/watch?v=O-3uPB3fgys");
    v.setLastNewExerciseMuscleArea(0);
    v.setLastNewExerciseMuscleArea(2);

    v.pushDownloadAllButton();

    e->accept();

    std::string expected_exercises_xml = 
    "<EXERCISES>\n"
    "  <EXERCISE>\n"
    "    <NAME>\n"
    "      Situp\n"
    "    </NAME>\n"
    "    <VIDEOURL>\n"
    "      https://www.youtube.com/watch?v=O-3uPB3fgys\n"
    "    </VIDEOURL>\n"
    "    <VIDEOPATH>\n"
    "      ${CMAKE_SOURCE_DIR}/build/test/Situp_50_53_resized.mp4\n"
    "    </VIDEOPATH>\n"
    "    <THUMBNAILPATH>\n"
    "      ${CMAKE_SOURCE_DIR}/build/test/Situp_50_53.jpg\n"
    "    </THUMBNAILPATH>\n"
    "    <VIDEOSTARTTIME>\n"
    "      50\n"
    "    </VIDEOSTARTTIME>\n"
    "    <VIDEOENDTIME>\n"
    "      53\n"
    "    </VIDEOENDTIME>\n"
    "    <TRAINEDAREAS>\n"
    "      <AREA>\n"
    "        0\n"
    "      </AREA>\n"
    "      <AREA>\n"
    "        2\n"
    "      </AREA>\n"
    "    </TRAINEDAREAS>\n"
    "  </EXERCISE>\n"
    "  <EXERCISE>\n"
    "    <NAME>\n"
    "      SomethingElse\n"
    "    </NAME>\n"
    "    <VIDEOURL>\n"
    "      https://www.youtube.com/watch?v=O-3uPB3fgys\n"
    "    </VIDEOURL>\n"
    "    <VIDEOPATH>\n"
    "      ${CMAKE_SOURCE_DIR}/build/test/SomethingElse_49_54_resized.mp4\n"
    "    </VIDEOPATH>\n"
    "    <THUMBNAILPATH>\n"
    "      ${CMAKE_SOURCE_DIR}/build/test/SomethingElse_49_54.jpg\n"
    "    </THUMBNAILPATH>\n"
    "    <VIDEOSTARTTIME>\n"
    "      49\n"
    "    </VIDEOSTARTTIME>\n"
    "    <VIDEOENDTIME>\n"
    "      54\n"
    "    </VIDEOENDTIME>\n"
    "    <TRAINEDAREAS>\n"
    "      <AREA>\n"
    "        0\n"
    "      </AREA>\n"
    "      <AREA>\n"
    "        2\n"
    "      </AREA>\n"
    "    </TRAINEDAREAS>\n"
    "  </EXERCISE>\n"
    "</EXERCISES>\n";

    std::ifstream f(out_path);
    std::stringstream ss;
    ss << f.rdbuf();
    ASSERT_EQ(ss.str(),expected_exercises_xml);
}

TEST(ExerciseEditorBrowser, Init)
{
    FreeFit::GUI::ExerciseEditorBrowser b;
    b.show();
}

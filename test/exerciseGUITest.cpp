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

class ExerciseEditor : public ::testing::Test
{
    protected:
    FreeFit::Data::Profile p;

    virtual void SetUp()
    {
        p.setName("John Doe");
        p.setDateLastWorkout("31.12.2000");
        p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/exercises.xml");
        p.setPerformanceFactor(1.0);
        p.setPicturePath("none");
    }
};

TEST_F(ExerciseEditor,LaunchEditor)
{
    p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/LaunchEditor.xml");
    FreeFit::GUI::ExerciseEditor* d = new FreeFit::GUI::ExerciseEditor(p);
}

TEST_F(ExerciseEditor,AddButton)
{
    p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/AddButton.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.pushAddButton();
    int n_exercises = v.getNumberOfExercises();    
    ASSERT_EQ(n_exercises,2);
}

TEST_F(ExerciseEditor,DeleteButton)
{
    p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/DeleteButton.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.pushAddButton();
    v.pushAddButton();
    v.pushFirstDeleteButton();
    int n_exercises = v.getNumberOfExercises();    
    ASSERT_EQ(n_exercises,2);
}

TEST_F(ExerciseEditor,ExerciseDemand)
{
    p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/ExerciseDemand.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    std::shared_ptr<FreeFit::GUI::DownloadExerciseDemand> d = v.getFirstExerciseDemand();
    ASSERT_EQ(d->name,"...");
    ASSERT_EQ(d->video_url,"...");
    ASSERT_EQ(d->video_start_time,"...");
    ASSERT_EQ(d->video_end_time,"...");
}

TEST_F(ExerciseEditor,NonStandardInput)
{
    std::string ex_name = "Pushup";
    std::string ex_url = "https://www.youtube.com/watch?v=IODxDxX7oi4";
    std::string ex_start = "2";
    std::string ex_end = "3";
    
    p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/NonStandardInput.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.setFirstExerciseNameText(ex_name);
    v.setFirstExerciseURLText(ex_url);
    v.setFirstExerciseStartTimeText(ex_start);
    v.setFirstExerciseStopTimeText(ex_end);
    v.setFirstExerciseMuscleArea(0);
    v.setFirstExerciseMuscleArea(2);

    std::shared_ptr<FreeFit::GUI::DownloadExerciseDemand> d = v.getFirstExerciseDemand();

    auto it = d->muscle_areas.begin();
    ASSERT_EQ(d->name,ex_name);
    ASSERT_EQ(d->video_url,ex_url);
    ASSERT_EQ(d->video_start_time,ex_start);
    ASSERT_EQ(d->video_end_time,ex_end);
    ASSERT_EQ(*it,"Shoulder");
    ASSERT_EQ(*(++it),"Chest");
}

TEST_F(ExerciseEditor,ValidateFunctionTrue)
{
    std::string ex_name = "Pushup";
    std::string ex_url = "https://www.youtube.com/watch?v=IODxDxX7oi4";
    std::string ex_start = "2";
    std::string ex_end = "3";
    
    p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/ValidateFunctionTrue.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.setFirstExerciseNameText(ex_name);
    v.setFirstExerciseURLText(ex_url);
    v.setFirstExerciseStartTimeText(ex_start);
    v.setFirstExerciseStopTimeText(ex_end);

    ASSERT_TRUE(v.isFirstExerciseNameValid());
    ASSERT_TRUE(v.isFirstExerciseURLValid());
    ASSERT_TRUE(v.isFirstExerciseStartTimeValid());
    ASSERT_TRUE(v.isFirstExerciseStopTimeValid());
}

TEST_F(ExerciseEditor,ValidateFunctionNameWrong)
{
    p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/ValidateFunctionNameWrong.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);

    v.setFirstExerciseNameText("Pushup!");
    ASSERT_FALSE(v.isFirstExerciseNameValid());

    v.setFirstExerciseNameText("Pushupß");
    ASSERT_FALSE(v.isFirstExerciseNameValid());
    
    std::string dump;
    for (unsigned int i = 1; i <= 257;i++)
        dump += "a";
    v.setFirstExerciseNameText(dump);
    ASSERT_FALSE(v.isFirstExerciseNameValid());
}

TEST_F(ExerciseEditor,ValidateFunctionURLWrong)
{
    p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/ValidateFunctionURLWrong.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);

    v.setFirstExerciseURLText("https://www.NOTyoutube.com/watch?v=BxIUwbb1Nzg");
    ASSERT_FALSE(v.isFirstExerciseURLValid());

    v.setFirstExerciseURLText("http://www.NOTyoutube.com/watch?v=BxIUwbb1Nzg");
    ASSERT_FALSE(v.isFirstExerciseURLValid());

    v.setFirstExerciseURLText("https://www.youtube.com/embed/nyiNSFp2uf0");
    ASSERT_FALSE(v.isFirstExerciseURLValid());
}

TEST_F(ExerciseEditor,ValidateFunctionStartTimeWrong)
{
    p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/ValidateFunctionStartTimeWrong.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);

    v.setFirstExerciseStartTimeText("abc");
    ASSERT_FALSE(v.isFirstExerciseStartTimeValid());

    v.setFirstExerciseStartTimeText("12!");
    ASSERT_FALSE(v.isFirstExerciseStartTimeValid());

    v.setFirstExerciseStartTimeText("1234");
    ASSERT_FALSE(v.isFirstExerciseStartTimeValid());
}

TEST_F(ExerciseEditor,ValidateFunctionStopTimeWrong)
{
    p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/ValidateFunctionStopTimeWrong.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);

    v.setFirstExerciseStopTimeText("abc");
    ASSERT_FALSE(v.isFirstExerciseStopTimeValid());

    v.setFirstExerciseStopTimeText("12!");
    ASSERT_FALSE(v.isFirstExerciseStopTimeValid());

    v.setFirstExerciseStopTimeText("1234");
    ASSERT_FALSE(v.isFirstExerciseStopTimeValid());
}

TEST_F(ExerciseEditor,DownloadClickedCheckDemandContent)
{
    std::string ex_name = "Pushup";
    std::string ex_url = "https://www.youtube.com/watch?v=IODxDxX7oi4";
    std::string ex_start = "2";
    std::string ex_end = "3";
    
    p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/DownloadClickedCheckDemandContent.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.setFirstExerciseNameText(ex_name);
    v.setFirstExerciseURLText(ex_url);
    v.setFirstExerciseStartTimeText(ex_start);
    v.setFirstExerciseStopTimeText(ex_end);
    v.setFirstExerciseMuscleArea(0);
    v.setFirstExerciseMuscleArea(2);

    v.connectToDownloadSignalsOfItems();
    v.pushFirstDownloadButton();

    FreeFit::GUI::DownloadExerciseDemand d = v.getLastDemand();

    auto it = d.muscle_areas.begin();
    ASSERT_EQ(d.name,ex_name);
    ASSERT_EQ(d.video_url,ex_url);
    ASSERT_EQ(d.video_start_time,ex_start);
    ASSERT_EQ(d.video_end_time,ex_end);
    ASSERT_EQ(*it,"Shoulder");
    ASSERT_EQ(*(++it),"Chest");
}

TEST_F(ExerciseEditor,DownloadClickedCheckFileExists)
{
    std::string ex_name = "Pushup";
    std::string ex_url = "https://www.youtube.com/watch?v=IODxDxX7oi4";
    std::string ex_start = "2";
    std::string ex_end = "3";
    
    p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/DownloadClickedCheckFileExists.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.setFirstExerciseNameText(ex_name);
    v.setFirstExerciseURLText(ex_url);
    v.setFirstExerciseStartTimeText(ex_start);
    v.setFirstExerciseStopTimeText(ex_end);
    v.setFirstExerciseMuscleArea(0);
    v.setFirstExerciseMuscleArea(2);

    v.connectToDownloadSignalsOfItems();
    v.pushFirstDownloadButton();

    std::ifstream f("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/Pushup_2_3.mp4");
    ASSERT_TRUE(f.is_open());
}

TEST_F(ExerciseEditor,ReadXMLAndPopulateExerciseList)
{
    p.setPathToExerciseDB("/Users/mathias/Documents/programming_workspace/FreeFit/test/ReadXMLAndPopulateExerciseList.xml");
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    ASSERT_EQ(v.getNumberOfExercises(),2);
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
    std::string out_path = "/Users/mathias/Documents/programming_workspace/FreeFit/build/test/Pushup_35_40.mp4";
    std::ifstream f(out_path);
    ASSERT_EQ(e.getVideoPath(),out_path);
    ASSERT_TRUE(f.is_open());
}

TEST_F(ExerciseEditor, XMLOutput)
{
    std::string ex_name = "Situp";
    std::string ex_url = "https://www.youtube.com/watch?v=O-3uPB3fgys";
    std::string ex_start = "50";
    std::string ex_end = "53";
    
    std::string out_path = "/Users/mathias/Documents/programming_workspace/FreeFit/build/test/XMLOutput.xml";
    const int result = std::remove(out_path.c_str());
    p.setPathToExerciseDB(out_path);
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);

    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.setFirstExerciseNameText(ex_name);
    v.setFirstExerciseURLText(ex_url);
    v.setFirstExerciseStartTimeText(ex_start);
    v.setFirstExerciseStopTimeText(ex_end);
    v.setFirstExerciseMuscleArea(0);
    v.setFirstExerciseMuscleArea(2);

    v.connectToDownloadSignalsOfItems();
    v.pushFirstDownloadButton();

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
    "      /Users/mathias/Documents/programming_workspace/FreeFit/build/test/Situp_50_53.mp4\n"
    "    </VIDEOPATH>\n"
    "    <THUMBNAILPATH>\n"
    "      /Users/mathias/Documents/programming_workspace/FreeFit/build/test/Situp_50_53.jpg\n"
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

    std::ifstream f1("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/Situp_50_53_resized.mp4");
    std::ifstream f2("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/Situp_50_53.mp4");
    std::ifstream f3(out_path);
    std::stringstream ss;
    ss << f3.rdbuf();

    ASSERT_TRUE(f1.is_open());
    ASSERT_TRUE(f2.is_open());
    ASSERT_EQ(ss.str(),expected_exercises_xml);
}

TEST_F(ExerciseEditor, AddExerciseToExistingXML)
{
    std::string out_path = "/Users/mathias/Documents/programming_workspace/FreeFit/build/test/AddExerciseToExistingXML.xml";
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
    "      /Users/mathias/Documents/programming_workspace/FreeFit/build/test/Situp_50_53.mp4\n"
    "    </VIDEOPATH>\n"
    "    <THUMBNAILPATH>\n"
    "      /Users/mathias/Documents/programming_workspace/FreeFit/build/test/Situp_50_53.jpg\n"
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
    v.setLastExerciseNameText("SomethingElse");
    v.setLastExerciseURLText("https://www.youtube.com/watch?v=O-3uPB3fgys");
    v.setLastExerciseStartTimeText("49");
    v.setLastExerciseStopTimeText("54");
    v.setLastExerciseMuscleArea(0);
    v.setLastExerciseMuscleArea(2);
    
    v.pushFirstDownloadButton();
    v.pushLastDownloadButton();
    
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
    "      /Users/mathias/Documents/programming_workspace/FreeFit/build/test/Situp_50_53.mp4\n"
    "    </VIDEOPATH>\n"
    "    <THUMBNAILPATH>\n"
    "      /Users/mathias/Documents/programming_workspace/FreeFit/build/test/Situp_50_53.jpg\n"
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
    "      /Users/mathias/Documents/programming_workspace/FreeFit/build/test/SomethingElse_49_54.mp4\n"
    "    </VIDEOPATH>\n"
    "    <THUMBNAILPATH>\n"
    "      /Users/mathias/Documents/programming_workspace/FreeFit/build/test/SomethingElse_49_54.jpg\n"
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

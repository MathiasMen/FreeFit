#include <gtest/gtest.h>

#include <memory>

#include <QDialog>

#include "include/exerciseeditor.h"
#include "include/newexercisedemand.h"
#include "include/profile.h"
#include "include/newexercisedemandhandler.h"

int my_argc;
char** my_argv;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    my_argc = argc;
    my_argv = argv;
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
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ExerciseEditor* d = new FreeFit::GUI::ExerciseEditor(p);
}

TEST_F(ExerciseEditor,AddButton)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.pushAddButton();
    int n_exercises = v.getNumberOfExercises();    
    ASSERT_EQ(n_exercises,2);
}

TEST_F(ExerciseEditor,DeleteButton)
{
    QApplication a(my_argc,my_argv);
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
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    FreeFit::GUI::NewExerciseDemand* d = v.getFirstExerciseDemand();
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
    
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::ExerciseEditor* e = new FreeFit::GUI::ExerciseEditor(p);
    FreeFit::GUI::ExerciseEditorValidator v(e);
    v.setFirstExerciseNameText(ex_name);
    v.setFirstExerciseURLText(ex_url);
    v.setFirstExerciseStartTimeText(ex_start);
    v.setFirstExerciseStopTimeText(ex_end);
    v.setFirstExerciseMuscleArea(0);
    v.setFirstExerciseMuscleArea(2);

    FreeFit::GUI::NewExerciseDemand* d = v.getFirstExerciseDemand();

    auto it = d->muscle_areas.begin();
    ASSERT_EQ(d->name,ex_name);
    ASSERT_EQ(d->video_url,ex_url);
    ASSERT_EQ(d->video_start_time,ex_start);
    ASSERT_EQ(d->video_end_time,ex_end);
    ASSERT_EQ(*it,"Shoulder");
    ASSERT_EQ(*(++it),"MiddleBack");
}

TEST_F(ExerciseEditor,DownloadClickedCheckDemandContent)
{
    std::string ex_name = "Pushup";
    std::string ex_url = "https://www.youtube.com/watch?v=IODxDxX7oi4";
    std::string ex_start = "2";
    std::string ex_end = "3";
    
    QApplication a(my_argc,my_argv);
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

    FreeFit::GUI::NewExerciseDemand d = v.getLastDemand();

    auto it = d.muscle_areas.begin();
    ASSERT_EQ(d.name,ex_name);
    ASSERT_EQ(d.video_url,ex_url);
    ASSERT_EQ(d.video_start_time,ex_start);
    ASSERT_EQ(d.video_end_time,ex_end);
    ASSERT_EQ(*it,"Shoulder");
    ASSERT_EQ(*(++it),"MiddleBack");
}

TEST(NewExerciseDemandHandler, Init)
{
    FreeFit::Data::NewExerciseDemandHandler h("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/exercises.xml");

    FreeFit::Data::NewExerciseDemandHandlerValidator v(&h);

    ASSERT_EQ(v.getPathToDB(),"/Users/mathias/Documents/programming_workspace/FreeFit/build/test/exercises.xml");
    ASSERT_NE(v.getXMLReader(),nullptr);
    ASSERT_NE(v.getExerciseWriter(),nullptr);
    ASSERT_NE(v.getYoutubeDownloader(),nullptr);
}

TEST(NewExerciseDemandHandler, AddDemand)
{
    FreeFit::Data::NewExerciseDemandHandler h("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/AddDemandTestExercises.xml");
    FreeFit::Data::NewExerciseDemandHandlerValidator v(&h);
    std::shared_ptr<FreeFit::GUI::NewExerciseDemand> d = std::make_shared<FreeFit::GUI::NewExerciseDemand>();
    d->name = "Pushup";
    d->video_url = "https://www.youtube.com/watch?v=IODxDxX7oi4&t=35s";
    d->video_start_time = "35";
    d->video_end_time = "40";
    d->muscle_areas.push_back("Shoulder");
    d->muscle_areas.push_back("Chest");
    h.addDemand(d);
    ASSERT_EQ(v.getNumberOfDemands(),1);
}

TEST(NewExerciseDemandHandler, AddExecuteDemand)
{
    FreeFit::Data::NewExerciseDemandHandler h("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/AddExecuteDemand.xml");
    std::shared_ptr<FreeFit::GUI::NewExerciseDemand> d = std::make_shared<FreeFit::GUI::NewExerciseDemand>();
    d->name = "Situp";
    d->video_url = "https://www.youtube.com/watch?v=O-3uPB3fgys";
    d->video_start_time = "50";
    d->video_end_time = "53";
    d->muscle_areas.push_back("Shoulder");
    d->muscle_areas.push_back("Chest");
    h.addDemand(d);
    h.executeDemands();

    std::string expected_exercises_xml = 
    "<EXERCISES>\n"
    "  <EXERCISE>\n"
    "    <NAME>\n"
    "      Situp\n"
    "    </NAME>\n"
    "    <VIDEOPATH>\n"
    "      /Users/mathias/Documents/programming_workspace/FreeFit/build/test/Situp_50_53.mp4\n"
    "    </VIDEOPATH>\n"
    "    <TRAINEDAREAS>\n"
    "      <AREA>\n"
    "        0\n"
    "      </AREA>\n"
    "      <AREA>\n"
    "        4\n"
    "      </AREA>\n"
    "    </TRAINEDAREAS>\n"
    "  </EXERCISE>\n"
    "</EXERCISES>\n";

    std::ifstream f1("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/Situp_50_53_resized.mp4");
    std::ifstream f2("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/Situp_50_53.mp4");
    std::ifstream f3("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/AddExecuteDemand.xml");
    std::stringstream ss;
    ss << f3.rdbuf();

    ASSERT_TRUE(f1.is_open());
    ASSERT_TRUE(f2.is_open());
    ASSERT_EQ(ss.str(),expected_exercises_xml);
}

TEST(NewExerciseDemandHandler, AddExercisesToExistingXML)
{
    FreeFit::Data::NewExerciseDemandHandler h("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/AddExercisesToExistingXML.xml");
    std::shared_ptr<FreeFit::GUI::NewExerciseDemand> d = std::make_shared<FreeFit::GUI::NewExerciseDemand>();
    d->name = "Situp";
    d->video_url = "https://www.youtube.com/watch?v=O-3uPB3fgys";
    d->video_start_time = "50";
    d->video_end_time = "53";
    d->muscle_areas.push_back("Shoulder");
    d->muscle_areas.push_back("Chest");
    h.addDemand(d);
    h.executeDemands();

    h.addDemand(d);
    h.executeDemands();

    std::string expected_exercises_xml = 
    "<EXERCISES>\n"
    "  <EXERCISE>\n"
    "    <NAME>\n"
    "      Situp\n"
    "    </NAME>\n"
    "    <VIDEOPATH>\n"
    "      /Users/mathias/Documents/programming_workspace/FreeFit/build/test/Situp_50_53.mp4\n"
    "    </VIDEOPATH>\n"
    "    <TRAINEDAREAS>\n"
    "      <AREA>\n"
    "        0\n"
    "      </AREA>\n"
    "      <AREA>\n"
    "        4\n"
    "      </AREA>\n"
    "    </TRAINEDAREAS>\n"
    "  </EXERCISE>\n"
    "  <EXERCISE>\n"
    "    <NAME>\n"
    "      Situp\n"
    "    </NAME>\n"
    "    <VIDEOPATH>\n"
    "      /Users/mathias/Documents/programming_workspace/FreeFit/build/test/Situp_50_53.mp4\n"
    "    </VIDEOPATH>\n"
    "    <TRAINEDAREAS>\n"
    "      <AREA>\n"
    "        0\n"
    "      </AREA>\n"
    "      <AREA>\n"
    "        4\n"
    "      </AREA>\n"
    "    </TRAINEDAREAS>\n"
    "  </EXERCISE>\n"
    "</EXERCISES>\n";

    std::ifstream f("/Users/mathias/Documents/programming_workspace/FreeFit/build/test/AddExercisesToExistingXML.xml");
    std::stringstream ss;
    ss << f.rdbuf();
    ASSERT_EQ(ss.str(),expected_exercises_xml);
}
#include <memory>
#include <fstream>
#include <sstream>
#include <gtest/gtest.h>
#include "include/xmlnode.h"
#include "include/xmlwriter.h"
#include "include/xmlreader.h"
#include "include/parser.h"
#include <regex>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class NodeTest : public ::testing::Test
{
    protected:
        std::shared_ptr<FreeFit::Data::XMLNode> n = std::make_shared<FreeFit::Data::XMLNode>(nullptr, "ROOT","");
};

TEST_F(NodeTest, FindChild)
{
    std::shared_ptr<FreeFit::Data::XMLNode> child = std::make_shared<FreeFit::Data::XMLNode>(n,"CHILD","SomeChild");
    n->addChild(child);
    std::shared_ptr<FreeFit::Data::XMLNode> found_child = n->findFirstChild("CHILD");
    ASSERT_EQ(child,found_child);
}

TEST_F(NodeTest, ModifyChildValue)
{
    std::shared_ptr<FreeFit::Data::XMLNode> child = std::make_shared<FreeFit::Data::XMLNode>(n,"CHILD","SomeChild");
    n->addChild(child);
    std::shared_ptr<FreeFit::Data::XMLNode> found_child = n->findFirstChild("CHILD");
    found_child->setValue("IAmAChild");
    ASSERT_EQ(found_child->getValue(),"IAmAChild");
}

TEST_F(NodeTest, FindChildren)
{
    std::shared_ptr<FreeFit::Data::XMLNode> child1 = std::make_shared<FreeFit::Data::XMLNode>(n,"CHILD_NAME1","SomeChild");
    std::shared_ptr<FreeFit::Data::XMLNode> child2 = std::make_shared<FreeFit::Data::XMLNode>(n,"CHILD_NAME2","SomeChild");
    std::shared_ptr<FreeFit::Data::XMLNode> child3 = std::make_shared<FreeFit::Data::XMLNode>(n,"CHILD_NAME1","SomeChild");

    n->addChild(child1);
    n->addChild(child2);
    n->addChild(child3);
    std::list<std::shared_ptr<FreeFit::Data::XMLNode>> l = n->findAllChildren("CHILD_NAME1");
    ASSERT_EQ(l.size(),2);
}

class ExerciseXMLTest : public ::testing::Test
{
    protected:
        std::string out_path = "${CMAKE_BINARY_DIR}/test/WriteExerciseFileTest.xml";
};

TEST_F(ExerciseXMLTest, WriteXMLFile)
{
    std::string expected = 
    "<EXERCISES>\n"
    "  <EXERCISE>\n"
    "    <NAME>\n"
    "      TestExercise\n"
    "    </NAME>\n"
    "    <VIDEOPATH>\n"
    "    </VIDEOPATH>\n"
    "    <BASEVOL>\n"
    "      20\n"
    "    </BASEVOL>\n"
    "    <TYPE>\n"
    "      1\n"
    "    </TYPE>\n"
    "    <TRAINEDAREAS>\n"
    "      <AREA>\n"
    "        0\n"
    "      </AREA>\n"
    "      <AREA>\n"
    "        5\n"
    "      </AREA>\n"
    "      <AREA>\n"
    "        10\n"
    "      </AREA>\n"
    "    </TRAINEDAREAS>\n"
    "  </EXERCISE>\n"
    "</EXERCISES>\n";

    FreeFit::Data::Exercise e;
    e.setName("TestExercise");
    e.setBaseVolume(20);
    e.setExerciseType(FreeFit::Data::ExerciseType::RepetitionBased);
    e.addTrainedMuscle(FreeFit::Data::MuscleGroup::Shoulder);
    e.addTrainedMuscle(FreeFit::Data::MuscleGroup::Biceps);
    e.addTrainedMuscle(FreeFit::Data::MuscleGroup::Shoulder);
    e.addTrainedMuscle(FreeFit::Data::MuscleGroup::Glutes);

    std::list<FreeFit::Data::Exercise> l {e};
    FreeFit::Data::ExerciseWriter w(out_path);
    w.createNodeTree(l);
    w.write();

    std::ifstream f(out_path);
    std::stringstream ss;
    ss << f.rdbuf();

    ASSERT_EQ(ss.str(),expected);
}

TEST_F(ExerciseXMLTest, ReadXML)
{
    FreeFit::Data::BaseXMLReader r(out_path);
    std::shared_ptr<FreeFit::Data::XMLNode> pt = r.read();
    ASSERT_EQ(pt->findFirstChild("EXERCISE")->findFirstChild("NAME")->getValue(),"TestExercise");
}

TEST_F(ExerciseXMLTest, ParseXMLNodeTree)
{
    FreeFit::Data::Exercise e1,e2;
    e1.setName("TestExercise1");
    e1.setBaseVolume(20);
    e1.setExerciseType(FreeFit::Data::ExerciseType::RepetitionBased);
    e1.addTrainedMuscle(FreeFit::Data::MuscleGroup::Shoulder);
    e1.addTrainedMuscle(FreeFit::Data::MuscleGroup::Biceps);

    e2.setName("TestExercise2");
    e2.setBaseVolume(30);
    e2.setExerciseType(FreeFit::Data::ExerciseType::TimeBased);
    e2.addTrainedMuscle(FreeFit::Data::MuscleGroup::Calves);
    e2.addTrainedMuscle(FreeFit::Data::MuscleGroup::Glutes);

    std::list<FreeFit::Data::Exercise> l_out {e1,e2};
    FreeFit::Data::ExerciseWriter w(out_path);
    w.createNodeTree(l_out);
    w.write();

    FreeFit::Data::BaseXMLReader r(out_path);
    std::shared_ptr<FreeFit::Data::XMLNode> pt = r.read();

    FreeFit::Data::ExerciseTreeParser p;
    std::list<FreeFit::Data::Exercise> l_in = p.parse(pt);
    ASSERT_EQ(l_in.begin()->getName(),"TestExercise1");
    ASSERT_EQ(l_in.begin()->getBaseVolume(),20);
    ASSERT_EQ(l_in.begin()->getExerciseType(),FreeFit::Data::ExerciseType::RepetitionBased);
    ASSERT_NE(l_in.begin()->getTrainedMuscles().count(FreeFit::Data::MuscleGroup::Biceps),0);

    ASSERT_EQ(l_in.rbegin()->getName(),"TestExercise2");
    ASSERT_EQ(l_in.rbegin()->getBaseVolume(),30);
    ASSERT_EQ(l_in.rbegin()->getExerciseType(),FreeFit::Data::ExerciseType::TimeBased);
    ASSERT_NE(l_in.rbegin()->getTrainedMuscles().count(FreeFit::Data::MuscleGroup::Glutes),0);
}

class ProfileXMLTest : public ::testing::Test
{
    protected:
        std::string out_path = "${CMAKE_BINARY_DIR}/test/WriteProfileFileTest.xml";
};

TEST_F(ProfileXMLTest, WriteXMLFile)
{
    std::string expected = 
    "<PROFILE>\n"
    "  <NAME>\n"
    "    TestProfile\n"
    "  </NAME>\n"
    "  <PICTUREPATH>\n"
    "    DummyPicturePath\n"
    "  </PICTUREPATH>\n"
    "  <DATELASTWORKOUT>\n"
    "    24.03.1994\n"
    "  </DATELASTWORKOUT>\n"
    "  <PATHEXDB>\n"
    "    DummyExercisesPath\n"
    "  </PATHEXDB>\n"
    "  <PERFFACTOR>\n"
    "    10.000000\n"
    "  </PERFFACTOR>\n"
    "</PROFILE>\n";

    FreeFit::Data::Profile p;
    p.setName("TestProfile");
    p.setPicturePath("DummyPicturePath");
    p.setDateLastWorkout("24.03.1994");
    p.setPathToExerciseDB("DummyExercisesPath");
    p.setPerformanceFactor(10.0);

    FreeFit::Data::ProfileWriter w(out_path);
    w.createNodeTree(p);
    w.write();

    std::ifstream f(out_path);
    std::stringstream ss;
    ss << f.rdbuf();

    ASSERT_EQ(ss.str(),expected);
}

TEST_F(ProfileXMLTest, ReadXML)
{
    FreeFit::Data::BaseXMLReader r(out_path);
    std::shared_ptr<FreeFit::Data::XMLNode> pt = r.read();
    ASSERT_EQ(pt->findFirstChild("NAME")->getValue(),"TestProfile");
}

TEST_F(ProfileXMLTest, ParseXMLNodeTree)
{
    FreeFit::Data::Profile p_out;
    p_out.setName("TestProfile");
    p_out.setPicturePath("DummyPicturePath");
    p_out.setDateLastWorkout("24.03.1994");
    p_out.setPathToExerciseDB("DummyExercisesPath");
    p_out.setPerformanceFactor(10.0);

    FreeFit::Data::ProfileWriter w(out_path);
    w.createNodeTree(p_out);
    w.write();

    FreeFit::Data::BaseXMLReader r(out_path);
    std::shared_ptr<FreeFit::Data::XMLNode> pt = r.read();

    FreeFit::Data::ProfileTreeParser p;
    FreeFit::Data::Profile p_in = p.parse(pt);
    
    ASSERT_EQ(p_out.getName(),p_in.getName());
    ASSERT_EQ(p_out.getPicturePath(),p_in.getPicturePath());
    ASSERT_EQ(p_out.getDateLastWorkout(),p_in.getDateLastWorkout());
    ASSERT_EQ(p_out.getPathToExerciseDB(),p_in.getPathToExerciseDB());
    ASSERT_EQ(p_out.getPerformanceFactor(),p_in.getPerformanceFactor());    
}

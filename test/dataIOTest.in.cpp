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

class DataIO : public ::testing::Test
{
    protected:
        std::shared_ptr<FreeFit::Data::XMLNode> n = std::make_shared<FreeFit::Data::XMLNode>(nullptr, "ROOT","");
};

TEST_F(DataIO, NodeAddAndFindChild)
{
    std::shared_ptr<FreeFit::Data::XMLNode> child = std::make_shared<FreeFit::Data::XMLNode>(n,"CHILD","SomeChild");
    n->addChild(child);
    std::shared_ptr<FreeFit::Data::XMLNode> found_child = n->findFirstChild("CHILD");
    ASSERT_EQ(child,found_child);
}

TEST_F(DataIO, ModifyChildValue)
{
    std::shared_ptr<FreeFit::Data::XMLNode> child = std::make_shared<FreeFit::Data::XMLNode>(n,"CHILD","SomeChild");
    n->addChild(child);
    std::shared_ptr<FreeFit::Data::XMLNode> found_child = n->findFirstChild("CHILD");
    found_child->setValue("IAmAChild");
    ASSERT_EQ(found_child->getValue(),"IAmAChild");
}

TEST_F(DataIO, FindChildren)
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

TEST_F(DataIO, WriteExercisesFile)
{
    std::string out_path = "${CMAKE_BINARY_DIR}/test/WriteExerciseFileTest.xml";

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

TEST_F(DataIO, WriteProfileFile)
{
    std::string out_path = "${CMAKE_BINARY_DIR}/test/WriteProfileFileTest.xml";

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

TEST_F(DataIO, ReadXML1)
{
    std::string out_path = "${CMAKE_BINARY_DIR}/test/WriteProfileFileTest.xml";
    FreeFit::Data::BaseXMLReader r(out_path);
    std::shared_ptr<FreeFit::Data::XMLNode> pt = r.read();
    ASSERT_EQ(pt->findFirstChild("NAME")->getValue(),"TestProfile");
}

TEST_F(DataIO, ReadXML2)
{
    std::string out_path = "${CMAKE_BINARY_DIR}/test/WriteExerciseFileTest.xml";
    FreeFit::Data::BaseXMLReader r(out_path);
    std::shared_ptr<FreeFit::Data::XMLNode> pt = r.read();
    ASSERT_EQ(pt->findFirstChild("EXERCISE")->findFirstChild("NAME")->getValue(),"TestExercise");
}

TEST_F(DataIO, ExerciseTreeParser)
{
    std::string out_path = "${CMAKE_BINARY_DIR}/test/ExerciseTreeParser.xml";

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
    ASSERT_EQ(l_in.rbegin()->getBaseVolume(),30);
}

#include <memory>
#include <fstream>
#include <sstream>
#include <gtest/gtest.h>
#include "include/xmlnode.h"
#include "include/xmlwriter.h"

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

TEST_F(DataIO, NodeGetXMLString)
{
    std::shared_ptr<FreeFit::Data::XMLNode> child = std::make_shared<FreeFit::Data::XMLNode>(n,"CHILD","SomeChild");
    n->addChild(child);
    std::string res = 
        "<ROOT>\n"
        "<CHILD>\n"
        "SomeChild\n"
        "</CHILD>\n"
        "</ROOT>\n";
    std::string cmp;
    n->getXMLString(cmp);
    ASSERT_EQ(cmp,res);
}

TEST_F(DataIO, NodeFindAndGetXMLString)
{
    std::shared_ptr<FreeFit::Data::XMLNode> child = std::make_shared<FreeFit::Data::XMLNode>(n,"CHILD","SomeChild");
    n->addChild(child);
    std::shared_ptr<FreeFit::Data::XMLNode> found_child = n->findFirstChild("CHILD");
    std::string res = 
        "<CHILD>\n"
        "SomeChild\n"
        "</CHILD>\n";
    std::string cmp;
    found_child->getXMLString(cmp);
    ASSERT_EQ(cmp,res);
}

TEST_F(DataIO, ModifyChildValue)
{
    std::shared_ptr<FreeFit::Data::XMLNode> child = std::make_shared<FreeFit::Data::XMLNode>(n,"CHILD","SomeChild");
    n->addChild(child);
    std::shared_ptr<FreeFit::Data::XMLNode> found_child = n->findFirstChild("CHILD");
    found_child->setValue("IAmAChild");
    std::string res = 
        "<CHILD>\n"
        "IAmAChild\n"
        "</CHILD>\n";
    std::string cmp;
    found_child->getXMLString(cmp);
    ASSERT_EQ(cmp,res);
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
    "<ROOT>\n"
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
    "</ROOT>\n";


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
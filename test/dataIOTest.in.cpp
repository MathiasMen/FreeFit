#include <memory>
#include <fstream>
#include <sstream>
#include <gtest/gtest.h>
#include "include/xmlnode.h"
#include "include/xmlwriter.h"
#include "include/xmlreader.h"
#include "include/parser.h"
#include "include/exercise.h"
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
    std::shared_ptr<FreeFit::Data::XMLNode> child1 = std::make_shared<FreeFit::Data::XMLNode>(n,"CHILD_NAME1","Value1");
    std::shared_ptr<FreeFit::Data::XMLNode> child2 = std::make_shared<FreeFit::Data::XMLNode>(n,"CHILD_NAME2","Value2");
    std::shared_ptr<FreeFit::Data::XMLNode> child3 = std::make_shared<FreeFit::Data::XMLNode>(n,"CHILD_NAME1","Value3");

    n->addChild(child1);
    n->addChild(child2);
    n->addChild(child3);
    std::list<std::shared_ptr<FreeFit::Data::XMLNode>> l = n->findAllChildren("CHILD_NAME1");
    ASSERT_EQ(l.size(),2);
}

TEST_F(NodeTest, EqualityOperatorTrue)
{
    std::shared_ptr<FreeFit::Data::XMLNode> child1 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"NODENAME","NODEVALUE");
    std::shared_ptr<FreeFit::Data::XMLNode> child2 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"NODENAME","NODEVALUE");
    ASSERT_TRUE(*child1 == *child2);
}

TEST_F(NodeTest, EqualityOperatorFalse)
{
    std::shared_ptr<FreeFit::Data::XMLNode> child1 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"NODENAME","NODEVALUE");
    std::shared_ptr<FreeFit::Data::XMLNode> child2 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"NODENAME","ANOTHERNODEVALUE");
    ASSERT_FALSE(*child1 == *child2);
}

TEST_F(NodeTest, EqualityInChildrenTrue)
{
    std::shared_ptr<FreeFit::Data::XMLNode> root1 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"ROOT","VALUE");
    std::shared_ptr<FreeFit::Data::XMLNode> root2 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"ROOT","VALUE");
    std::shared_ptr<FreeFit::Data::XMLNode> child1 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"CHILD","VALUE");
    std::shared_ptr<FreeFit::Data::XMLNode> child2 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"CHILD","VALUE");

    root1->addChild(child1);
    root2->addChild(child2);
    ASSERT_TRUE(*root1 == *root2);
}

TEST_F(NodeTest, EqualityInChildrenFalse)
{
    std::shared_ptr<FreeFit::Data::XMLNode> root1 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"ROOT","VALUE");
    std::shared_ptr<FreeFit::Data::XMLNode> root2 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"ROOT","VALUE");
    std::shared_ptr<FreeFit::Data::XMLNode> child1 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"CHILD","VALUE");
    std::shared_ptr<FreeFit::Data::XMLNode> child2 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"CHILD","NOTVALUE");

    root1->addChild(child1);
    root2->addChild(child2);
    ASSERT_FALSE(*root1 == *root2);
}

TEST_F(NodeTest, AddExistingNodeWithChildren)
{
    std::shared_ptr<FreeFit::Data::XMLNode> root = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"ROOT","VALUE");
    std::shared_ptr<FreeFit::Data::XMLNode> child1 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"C","CHILDVALUE");
    std::shared_ptr<FreeFit::Data::XMLNode> child2 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"C","CHILDVALUE");
    std::shared_ptr<FreeFit::Data::XMLNode> grandChild11 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"GC","GCVALUE1");
    std::shared_ptr<FreeFit::Data::XMLNode> grandChild12 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"GC","GCVALUE2");
    std::shared_ptr<FreeFit::Data::XMLNode> grandChild21 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"GC","GCVALUE1");
    std::shared_ptr<FreeFit::Data::XMLNode> grandChild22 = std::make_shared<FreeFit::Data::XMLNode>(nullptr,"GC","GCVALUE2");

    child1->addChild(grandChild11);
    child1->addChild(grandChild12);
    child2->addChild(grandChild21);
    child2->addChild(grandChild22);
    root->addChild(child1);
    root->addChild(child2);
    ASSERT_EQ(root->getChildren().size(),1);
}

TEST_F(NodeTest, AddAlreadyExistingNode)
{
    std::shared_ptr<FreeFit::Data::XMLNode> child1 = std::make_shared<FreeFit::Data::XMLNode>(n,"NODENAME","NODEVALUE");
    std::shared_ptr<FreeFit::Data::XMLNode> child2 = std::make_shared<FreeFit::Data::XMLNode>(n,"NODENAME","NODEVALUE");
    n->addChild(child1);
    n->addChild(child2);
    ASSERT_EQ(n->getChildren().size(),1);
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
    "    <VIDEOURL>\n"
    "      https://www.youtube.com/watch?v=-kwe1EOiWMY\n"
    "    </VIDEOURL>\n"
    "    <VIDEOPATH>\n"
    "    </VIDEOPATH>\n"
    "    <VIDEOSTARTTIME>\n"
    "      2\n"
    "    </VIDEOSTARTTIME>\n"
    "    <VIDEOENDTIME>\n"
    "      5\n"
    "    </VIDEOENDTIME>\n"
    "    <TRAINEDAREAS>\n"
    "      <AREA>\n"
    "        0\n"
    "      </AREA>\n"
    "      <AREA>\n"
    "        1\n"
    "      </AREA>\n"
    "      <AREA>\n"
    "        5\n"
    "      </AREA>\n"
    "    </TRAINEDAREAS>\n"
    "  </EXERCISE>\n"
    "</EXERCISES>\n";

    FreeFit::Data::Exercise e;
    e.setName("TestExercise");
    e.setVideoURL("https://www.youtube.com/watch?v=-kwe1EOiWMY");
    e.addTrainedMuscle(FreeFit::Data::MuscleGroup::Shoulder);
    e.addTrainedMuscle(FreeFit::Data::MuscleGroup::Back);
    e.addTrainedMuscle(FreeFit::Data::MuscleGroup::Shoulder);
    e.addTrainedMuscle(FreeFit::Data::MuscleGroup::Legs);
    e.setVideoStartTime("2");
    e.setVideoEndTime("5");

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
    ASSERT_EQ(pt->findFirstChild("EXERCISE")->findFirstChild("VIDEOURL")->getValue(),"https://www.youtube.com/watch?v=-kwe1EOiWMY");
    ASSERT_EQ(pt->findFirstChild("EXERCISE")->findFirstChild("VIDEOSTARTTIME")->getValue(),"2");
    ASSERT_EQ(pt->findFirstChild("EXERCISE")->findFirstChild("VIDEOENDTIME")->getValue(),"5");
}

TEST_F(ExerciseXMLTest, ParseXMLNodeTree)
{
    FreeFit::Data::Exercise e1,e2;
    e1.setName("TestExercise1");
    e1.setVideoURL("https://www.youtube.com/watch?v=stuvwxyz123");
    e1.setVideoStartTime("1");
    e1.setVideoEndTime("2");
    e1.addTrainedMuscle(FreeFit::Data::MuscleGroup::Shoulder);
    e1.addTrainedMuscle(FreeFit::Data::MuscleGroup::Back);

    e2.setName("TestExercise2");
    e2.setVideoURL("https://www.youtube.com/watch?v=abcdefghijk");
    e2.setVideoStartTime("3");
    e2.setVideoEndTime("4");
    e2.addTrainedMuscle(FreeFit::Data::MuscleGroup::Arms);
    e2.addTrainedMuscle(FreeFit::Data::MuscleGroup::Legs);

    std::list<FreeFit::Data::Exercise> l_out {e1,e2};
    FreeFit::Data::ExerciseWriter w(out_path);
    w.createNodeTree(l_out);
    w.write();

    FreeFit::Data::BaseXMLReader r(out_path);
    std::shared_ptr<FreeFit::Data::XMLNode> pt = r.read();

    FreeFit::Data::ExerciseTreeParser p;
    std::list<FreeFit::Data::Exercise> l_in = p.parse(pt);

    ASSERT_EQ(l_in.begin()->getName(),"TestExercise1");
    ASSERT_EQ(l_in.begin()->getVideoURL(),"https://www.youtube.com/watch?v=stuvwxyz123");
    ASSERT_EQ(l_in.begin()->getVideoStartTime(),"1");
    ASSERT_EQ(l_in.begin()->getVideoEndTime(),"2");
    ASSERT_NE(l_in.begin()->getTrainedMuscles().count(FreeFit::Data::MuscleGroup::Back),0);

    ASSERT_EQ(l_in.rbegin()->getName(),"TestExercise2");
    ASSERT_EQ(l_in.rbegin()->getVideoURL(),"https://www.youtube.com/watch?v=abcdefghijk");
    ASSERT_EQ(l_in.rbegin()->getVideoStartTime(),"3");
    ASSERT_EQ(l_in.rbegin()->getVideoEndTime(),"4");
    ASSERT_NE(l_in.rbegin()->getTrainedMuscles().count(FreeFit::Data::MuscleGroup::Legs),0);
}

TEST_F(ExerciseXMLTest, ParseXMLNodeTreeAndReturnExerciseList)
{
    FreeFit::Data::Exercise e1,e2;
    e1.setName("TestExercise1");
    e1.setVideoURL("https://www.youtube.com/watch?v=stuvwxyz123");
    e1.setVideoStartTime("1");
    e1.setVideoEndTime("2");
    e1.addTrainedMuscle(FreeFit::Data::MuscleGroup::Shoulder);
    e1.addTrainedMuscle(FreeFit::Data::MuscleGroup::Back);

    e2.setName("TestExercise2");
    e2.setVideoURL("https://www.youtube.com/watch?v=abcdefghijk");
    e2.setVideoStartTime("3");
    e2.setVideoEndTime("4");
    e2.addTrainedMuscle(FreeFit::Data::MuscleGroup::Arms);
    e2.addTrainedMuscle(FreeFit::Data::MuscleGroup::Legs);

    std::list<FreeFit::Data::Exercise> l_out {e1,e2};
    FreeFit::Data::ExerciseWriter w(out_path);
    w.createNodeTree(l_out);
    w.write();

    FreeFit::Data::BaseXMLReader r(out_path);
    std::list<FreeFit::Data::Exercise> l_in = r.getExerciseList();

    ASSERT_EQ(l_out.begin()->getName(),l_in.begin()->getName());
    ASSERT_EQ(l_out.begin()->getVideoURL(),l_in.begin()->getVideoURL());
    ASSERT_EQ(l_out.begin()->getVideoStartTime(),l_in.begin()->getVideoStartTime());
    ASSERT_EQ(l_out.begin()->getVideoEndTime(),l_in.begin()->getVideoEndTime());

    ASSERT_EQ(l_out.rbegin()->getName(),l_in.rbegin()->getName());
    ASSERT_EQ(l_out.rbegin()->getVideoURL(),l_in.rbegin()->getVideoURL());
    ASSERT_EQ(l_out.rbegin()->getVideoStartTime(),l_in.rbegin()->getVideoStartTime());
    ASSERT_EQ(l_out.rbegin()->getVideoEndTime(),l_in.rbegin()->getVideoEndTime());
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

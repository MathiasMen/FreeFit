#include <gtest/gtest.h>

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

#include "include/profileitem.h"

int my_argc;
char** my_argv;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    my_argc = argc;
    my_argv = argv;
    return RUN_ALL_TESTS();
}

TEST(ProfileItemGroup,Launch)
{
    QApplication a(my_argc,my_argv);

    QWidget* w = new QWidget;
    QVBoxLayout* ly = new QVBoxLayout(w);
    FreeFit::GUI::ProfileItemGroup g;
    FreeFit::GUI::ProfileItem b1("Text1",w);
    FreeFit::GUI::ProfileItem b2("Text2",w);
    FreeFit::GUI::ProfileItem b3("Some really long text.",w);
    g.addItem(&b1);
    g.addItem(&b2);
    g.addItem(&b3);
    ly->addWidget(&b1);
    ly->addWidget(&b2);
    ly->addWidget(&b3);
    w->show();
    a.exec();
}

TEST(ProfileItemGroup,SelectionStyling)
{
    QApplication a(my_argc,my_argv);

    QWidget* w = new QWidget;
    QVBoxLayout* ly = new QVBoxLayout(w);
    FreeFit::GUI::ProfileItemGroup g;
    FreeFit::GUI::ProfileItemValidator v(&g);
    FreeFit::GUI::ProfileItem b1("Text1");
    FreeFit::GUI::ProfileItem b2("Text2");
    FreeFit::GUI::ProfileItem b3("Some really long text.");
    g.addItem(&b1);
    g.addItem(&b2);
    g.addItem(&b3);
    v.selectProfileItem(1);
    v.selectProfileItem(0);
    v.selectProfileItem(1);
    ly->addWidget(&b1);
    ly->addWidget(&b2);
    ly->addWidget(&b3);
    ASSERT_TRUE(v.getProfileCSSString(0).find("color:grey; border: 2px solid grey;") != std::string::npos);
    ASSERT_TRUE(v.getProfileCSSString(1).find("color:red; border: 2px solid red;") != std::string::npos);
}

TEST(ProfileItemGroup,ChangeSelectionAndEdit)
{
    QApplication a(my_argc,my_argv);

    QWidget* w = new QWidget;
    QVBoxLayout* ly = new QVBoxLayout(w);
    FreeFit::GUI::ProfileItemGroup g;
    FreeFit::GUI::ProfileItemValidator v(&g);
    FreeFit::GUI::ProfileItem b1("Mathias");
    FreeFit::GUI::ProfileItem b2("Constanze");
    FreeFit::GUI::ProfileItem b3("Uschi");
    g.addItem(&b1);
    g.addItem(&b2);
    g.addItem(&b3);
    v.selectProfileItem(1);
    v.selectProfileItem(0);
    v.selectProfileItem(1);
    ly->addWidget(&b1);
    ly->addWidget(&b2);
    ly->addWidget(&b3);
    g.setCurrentName("ConstanzeTest");
    ASSERT_EQ(v.getCurrentName(),"ConstanzeTest");
    ASSERT_EQ(v.getName(0),"Mathias");
    ASSERT_EQ(v.getName(1),"ConstanzeTest");
    ASSERT_EQ(v.getName(2),"Uschi");
}


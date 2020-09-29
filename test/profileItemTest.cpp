#include <gtest/gtest.h>

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QString>
#include <QColor>

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

    QString text1 = "Text1", text2 = "Text2", text3 = "Some really long text.";
    QColor color1("red"), color2("green"), color3("blue");
    QWidget* w = new QWidget;
    QVBoxLayout* ly = new QVBoxLayout(w);
    FreeFit::GUI::ProfileItemGroup g;
    FreeFit::GUI::ProfileItem b1(text1,color1,w);
    FreeFit::GUI::ProfileItem b2(text2,color2,w);
    FreeFit::GUI::ProfileItem b3(text3,color3,w);
    g.addItem(&b1);
    g.addItem(&b2);
    g.addItem(&b3);
    ly->addWidget(&b1);
    ly->addWidget(&b2);
    ly->addWidget(&b3);
    ASSERT_EQ(b1.getName(),text1.toStdString());
    ASSERT_EQ(b2.getName(),text2.toStdString());
    ASSERT_EQ(b3.getName(),text3.toStdString());
    ASSERT_EQ(b1.getColor(),color1.name());
    ASSERT_EQ(b2.getColor(),color2.name());
    ASSERT_EQ(b3.getColor(),color3.name());
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
    ASSERT_TRUE(v.getProfileCSSString(0).find("color:#808080; border: 2px solid #808080;") != std::string::npos);
    ASSERT_TRUE(v.getProfileCSSString(1).find("color:#ff0000; border: 2px solid #ff0000;") != std::string::npos);
    w->close();
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
    w->close();
}


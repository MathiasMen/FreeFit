#include <gtest/gtest.h>

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QString>
#include <QColor>

#include "include/profileitem.h"

#include <iostream>

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
    ASSERT_EQ(&g,b1.getGroupPointer());
}

TEST(ProfileItemGroup,SelectionStyling)
{
    QApplication a(my_argc,my_argv);

    QString text1 = "Text1", text2 = "Text2", text3 = "Some really long text.";
    QColor color1("green"), color2("blue"), color3("red");
    QWidget* w = new QWidget;
    QVBoxLayout* ly = new QVBoxLayout(w);
    FreeFit::GUI::ProfileItemGroup g;
    FreeFit::GUI::ProfileItemValidator v(&g);
    FreeFit::GUI::ProfileItem b1(text1,color1,w);
    FreeFit::GUI::ProfileItem b2(text2,color2,w);
    FreeFit::GUI::ProfileItem b3(text3,color3,w);
    g.addItem(&b1);
    g.addItem(&b2);
    g.addItem(&b3);
    ly->addWidget(&b1);
    ly->addWidget(&b2);
    ly->addWidget(&b3);
    v.selectProfileItem(1);
    ASSERT_TRUE(v.getProfileCSSString(0).find("color:#808080; border: 2px solid #808080;") != std::string::npos);
    ASSERT_TRUE(v.getProfileCSSString(1).find("color:#0000ff; border: 2px solid #0000ff;") != std::string::npos);
    ASSERT_TRUE(v.getProfileCSSString(2).find("color:#808080; border: 2px solid #808080;") != std::string::npos);
    v.selectProfileItem(0);
    ASSERT_TRUE(v.getProfileCSSString(0).find("color:#008000; border: 2px solid #008000;") != std::string::npos);
    ASSERT_TRUE(v.getProfileCSSString(1).find("color:#808080; border: 2px solid #808080;") != std::string::npos);
    ASSERT_TRUE(v.getProfileCSSString(2).find("color:#808080; border: 2px solid #808080;") != std::string::npos);
    v.selectProfileItem(1);
    ASSERT_TRUE(v.getProfileCSSString(0).find("color:#808080; border: 2px solid #808080;") != std::string::npos);
    ASSERT_TRUE(v.getProfileCSSString(1).find("color:#0000ff; border: 2px solid #0000ff;") != std::string::npos);
    ASSERT_TRUE(v.getProfileCSSString(2).find("color:#808080; border: 2px solid #808080;") != std::string::npos);
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

TEST(ProfileItemGroup,GetItems)
{
    QApplication a(my_argc,my_argv);

    FreeFit::GUI::ProfileItemGroup g;
    FreeFit::GUI::ProfileItemValidator v(&g);
    FreeFit::GUI::ProfileItem b1("Mathias");
    FreeFit::GUI::ProfileItem b2("Constanze");
    FreeFit::GUI::ProfileItem b3("Uschi");

    g.addItem(&b1);
    g.addItem(&b2);
    g.addItem(&b3);
    ASSERT_EQ(g.getItems().size(),3);
    ASSERT_EQ(g.getItems()[0],&b1);
    ASSERT_EQ(g.getItems()[1],&b2);
    ASSERT_EQ(g.getItems()[2],&b3);
}

TEST(ProfileItem,HandlePopupResult)
{
    QApplication a(my_argc,my_argv);

    QString text1 = "Text1", text2 = "Text2", text3 = "Some really long text.";
    QColor color1("red"), color2("green"), color3("blue");
    FreeFit::GUI::ProfileEditPopupResult res1("Text1Changed","",true), res2("Text2Changed","#0000ff",true), res3("Text3Changed","#ff0000",false);

    FreeFit::GUI::ProfileItemGroup g;
    FreeFit::GUI::ProfileItemValidator v(&g);
    FreeFit::GUI::ProfileItem b1(text1,color1);
    FreeFit::GUI::ProfileItem b2(text2,color2);
    FreeFit::GUI::ProfileItem b3(text3,color3);
    g.addItem(&b1);
    g.addItem(&b2);
    g.addItem(&b3);    

    ASSERT_EQ(b1.getName(),text1.toStdString());
    ASSERT_EQ(b2.getName(),text2.toStdString());
    ASSERT_EQ(b3.getName(),text3.toStdString());
    v.selectProfileItem(0);
    ASSERT_TRUE(v.getProfileCSSString(0).find("color:#ff0000; border: 2px solid #ff0000;") != std::string::npos);
    v.selectProfileItem(1);
    ASSERT_TRUE(v.getProfileCSSString(1).find("color:#008000; border: 2px solid #008000;") != std::string::npos);
    v.selectProfileItem(2);
    ASSERT_TRUE(v.getProfileCSSString(2).find("color:#0000ff; border: 2px solid #0000ff;") != std::string::npos);

    b1.handlePopupFinished(res1);
    b2.handlePopupFinished(res2);
    b3.handlePopupFinished(res3);

    ASSERT_EQ(b1.getName(),"Text1Changed");
    ASSERT_EQ(b2.getName(),"Text2Changed");
    ASSERT_EQ(b3.getName(),text3.toStdString());
    v.selectProfileItem(0);
    ASSERT_TRUE(v.getProfileCSSString(0).find("color:#ff0000; border: 2px solid #ff0000;") != std::string::npos);
    v.selectProfileItem(1);
    ASSERT_TRUE(v.getProfileCSSString(1).find("color:#0000ff; border: 2px solid #0000ff;") != std::string::npos);
    v.selectProfileItem(2);
    ASSERT_TRUE(v.getProfileCSSString(2).find("color:#ff0000; border: 2px solid #ff0000;") != std::string::npos);
}

TEST(ProfileItem,FontSize)
{
    QApplication a(my_argc,my_argv);

    QString text1 = "Text1";
    QString long_text = text1.repeated(100);
    QColor color1("red");

    QWidget* w = new QWidget;
    QVBoxLayout* ly = new QVBoxLayout(w);

    FreeFit::GUI::ProfileItemGroup g;
    FreeFit::GUI::ProfileItemValidator v(&g);
    FreeFit::GUI::ProfileItem b(long_text,color1);
    g.addItem(&b);
    ly->addWidget(&b);

    v.selectProfileItem(0);
    ASSERT_EQ(v.getCurrentFontSize(),8);
    ASSERT_EQ(v.getCurrentName(),long_text.toStdString());
    ASSERT_EQ(v.getCurrentDisplayName(),"Text1Text1Text1Text1Text1Text1Text1Text1T\xE2\x80\xA6");
}
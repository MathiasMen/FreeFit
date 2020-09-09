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

TEST(MaterialWidget,Launch)
{
    QApplication a(my_argc,my_argv);

    QWidget* w = new QWidget;
    QVBoxLayout* ly = new QVBoxLayout(w);
    FreeFit::GUI::ProfileItem b1("Text1",w);
    FreeFit::GUI::ProfileItem b2("Text2",w);
    FreeFit::GUI::ProfileItem b3("Some really long text.",w);
    ly->addWidget(&b1);
    ly->addWidget(&b2);
    ly->addWidget(&b3);
    w->show();
    a.exec();
}

TEST(MaterialWidget,ProfileItemGroup)
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

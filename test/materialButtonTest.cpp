#include <gtest/gtest.h>

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

#include "include/materialbutton.h"

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
    w->setStyleSheet("background-color:white");
    FreeFit::GUI::MaterialButton b1("Text1",w);
    FreeFit::GUI::MaterialButton b2("Text2",w);
    FreeFit::GUI::MaterialButton b3("Text3",w);
    ly->addWidget(&b1);
    ly->addWidget(&b2);
    ly->addWidget(&b3);
    w->show();
    a.exec();
}

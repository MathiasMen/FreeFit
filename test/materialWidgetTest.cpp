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
    FreeFit::GUI::MaterialButton* b = new FreeFit::GUI::MaterialButton(w);
    ly->addWidget(b);
    w->show();
    a.exec();
}

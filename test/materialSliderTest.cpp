#include <gtest/gtest.h>

#include <QApplication>

#include "include/materialslider.h"

int my_argc;
char** my_argv;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    my_argc = argc;
    my_argv = argv;
    return RUN_ALL_TESTS();
}

class materialSliderTest : public ::testing::Test
{
    protected:
    virtual void SetUp()
    {
    }
};

TEST_F(materialSliderTest,Init)
{
    QApplication a(my_argc,my_argv);
    FreeFit::GUI::MaterialSlider* s = new FreeFit::GUI::MaterialSlider(nullptr);
    s->show();
    a.exec();
}

#include <gtest/gtest.h>

#include <QApplication>

int my_argc;
char** my_argv;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    my_argc = argc;
    my_argv = argv;
    return RUN_ALL_TESTS();
}

class WorkoutGenerationWidgetTest : public ::testing::Test
{
    protected:
};

TEST_F(WorkoutGenerationWidgetTest,Launch)
{
    QApplication a(my_argc,my_argv);
}

#include <gtest/gtest.h>

#include <QDialog>

#include "include/exerciseeditor.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(ExerciseEditor,LaunchEditor)
{
    QDialog* d = new FreeFit::GUI::ExerciseEditor();
    d->exec();
}
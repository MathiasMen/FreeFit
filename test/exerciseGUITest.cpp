#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QDialog>

#include <iostream>

#include "include/exerciseeditor.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    QDialog* d = new FreeFit::GUI::ExerciseEditor();
    d->exec();
}

#pragma once

#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QListWidget>
#include <QGridLayout>
#include <QStyle>

#include <iostream>
#include <vector>

#include "include/newexercisedemand.h"

namespace FreeFit
{
    namespace GUI
    {
        class ExerciseEditor : public QDialog
        {
        public:
            ExerciseEditor()
                :   put_exercise_to_add_list(QApplication::style()->standardIcon(QStyle::SP_MediaSkipForward),"",this),
                    create_new_exercise(QApplication::style()->standardIcon(QStyle::SP_FileDialogNewFolder),"",this),
                    name("Enter exercise name...",this),
                    video_url("Enter video url...",this),
                    base_volume("Enter repetitions/time...",this),
                    exercise_type_group(this),
                    time_based_ex_button("Timebased Exercise",this),
                    rep_based_ex_button("Repetitionbased Exercise",this),
                    muscle_areas(this),
                    exercises_to_add(this)
            {
                exercise_type_group.addButton(&time_based_ex_button);
                exercise_type_group.addButton(&rep_based_ex_button);

                ly = new QGridLayout(this);
                int row_counter = -1;
                ly->addWidget(&name,++row_counter,1);
                ly->addWidget(&video_url,++row_counter,1);
                ly->addWidget(&base_volume,++row_counter,1);
                ly->addWidget(&time_based_ex_button,++row_counter,1);
                ly->addWidget(&rep_based_ex_button,row_counter,2);
                ly->addWidget(&muscle_areas,0,0,row_counter+1,1);
                ly->addWidget(&put_exercise_to_add_list,1,3);
                ly->addWidget(&create_new_exercise,2,3);
                ly->addWidget(&exercises_to_add,0,4,row_counter+1,1);
                this->setLayout(ly);
            }
        private:
            QPushButton put_exercise_to_add_list;
            QPushButton create_new_exercise;
            QLineEdit name;
            QLineEdit video_url;
            QLineEdit base_volume;
            QButtonGroup exercise_type_group;
            QRadioButton time_based_ex_button;
            QRadioButton rep_based_ex_button;
            QListWidget muscle_areas;
            QListWidget exercises_to_add;

            std::vector<NewExerciseDemand> exercise_demands;

            QGridLayout* ly;
        };
    }
}

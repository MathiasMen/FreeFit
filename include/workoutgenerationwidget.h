#pragma once

#include <memory>

#include <QDialog>
#include <QGridLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QIntValidator>
#include <QDialogButtonBox>

#include "include/exercise.h"
#include "include/workout.h"

namespace FreeFit
{
    namespace GUI
    {
        class WorkoutOption : public QRadioButton
        {
        public:
            WorkoutOption(QString text = "", QWidget* parent = nullptr) : QRadioButton(text,parent){}
        private:
            std::shared_ptr<FreeFit::Data::WorkoutBase> workout_data;
        };

        class WorkoutGenerationWidget : public QDialog
        {
        Q_OBJECT
        public:
            WorkoutGenerationWidget(QWidget* parent = nullptr) : QDialog(parent)
            {
                this->setWindowTitle("Workout details");
                this->resize(800,600);
                ly = new QGridLayout(this);

                number_of_lines = new QLineEdit("Enter number of rounds",this);
                int_validator = new QIntValidator(1,50,this);
                number_of_lines->setValidator(int_validator);
                ly->addWidget(number_of_lines,0,0);

                all_exercises_workout = new WorkoutOption("All Exercises",this);
                all_exercises_workout->setChecked(true);
                ly->addWidget(all_exercises_workout,1,0);

                button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
                connect(button_box, &QDialogButtonBox::accepted, this, &WorkoutGenerationWidget::accept);
                connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
                ly->addWidget(button_box,2,0);
                this->setLayout(ly);
            }

            void setPossibleExercises(std::list<FreeFit::Data::Exercise> e)
            {
                //for (auto w : workout_options)
                    //w->setPossibleExercises(e);
            }
        public slots:

            void accept() override
            {
                QDialog::accept();
            }

        private:
            QGridLayout* ly;
            WorkoutOption* all_exercises_workout;
            QIntValidator* int_validator;
            QLineEdit* number_of_lines;
            std::list<WorkoutOption> workout_options;
            QDialogButtonBox* button_box;
        };
    }
}
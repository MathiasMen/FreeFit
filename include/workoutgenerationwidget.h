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
        class WorkoutGenerationWidget : public QDialog
        {
        Q_OBJECT
        public:
            WorkoutGenerationWidget(QWidget* parent = nullptr) : QDialog(parent)
            {
                this->setWindowTitle("Workout details");
                ly = new QGridLayout(this);

                number_of_lines = new QLineEdit("Enter number of rounds",this);
                int_validator = new QIntValidator(1,50,this);
                number_of_lines->setValidator(int_validator);
                ly->addWidget(number_of_lines,0,0);

                workout_types.push_back(std::make_shared<FreeFit::Data::AllExercisesWorkout>(std::list<FreeFit::Data::Exercise>()));
                all_exercises_workout = new QRadioButton("All Exercises",this);
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
                for (auto w : workout_types)
                    w->setPossibleExercises(e);
            }
        public slots:

            void accept() override
            {
                QDialog::accept();
            }

        private:
            QGridLayout* ly;
            QRadioButton* all_exercises_workout;
            QIntValidator* int_validator;
            QLineEdit* number_of_lines;
            std::list<std::shared_ptr<FreeFit::Data::WorkoutBase>> workout_types;
            QDialogButtonBox* button_box;
        };
    }
}
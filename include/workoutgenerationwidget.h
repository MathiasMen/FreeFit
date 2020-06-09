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
            WorkoutOption(QString text = "", std::shared_ptr<FreeFit::Data::WorkoutBase> w = nullptr, QWidget* parent = nullptr) : QRadioButton(text,parent), workout_data(w){}

            void setPossibleExercises(std::list<FreeFit::Data::Exercise> e)
            {
                workout_data->setPossibleExercises(e);
            }
        
            std::list<FreeFit::Data::Exercise> getExercisesPerRound(){return workout_data->getExercisesPerRound();}

            unsigned int getRounds(){return workout_data->getRounds();}

            void generateWorkout(){workout_data->generate();}

            std::shared_ptr<FreeFit::Data::WorkoutBase> getWorkout(){return workout_data;}
        private:
            std::shared_ptr<FreeFit::Data::WorkoutBase> workout_data;
        };

        class WorkoutGenerationWidget : public QDialog
        {
        Q_OBJECT
        public:
            WorkoutGenerationWidget(QWidget* parent = nullptr) : QDialog(parent)
            {
                ly = new QGridLayout(this);

                number_of_lines = new QLineEdit("Enter number of rounds",this);
                int_validator = new QIntValidator(1,50,this);
                number_of_lines->setValidator(int_validator);
                ly->addWidget(number_of_lines,0,0);

                std::shared_ptr<FreeFit::Data::AllExercisesWorkout> w1 = std::make_shared<FreeFit::Data::AllExercisesWorkout>(std::list<FreeFit::Data::Exercise>());
                all_exercises_workout = new WorkoutOption("All Exercises",w1,this);
                all_exercises_workout->setChecked(true);
                ly->addWidget(all_exercises_workout,1,0);
                workout_options.push_back(all_exercises_workout);

                button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
                connect(button_box, &QDialogButtonBox::accepted, this, &WorkoutGenerationWidget::accept);
                connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
                ly->addWidget(button_box,2,0);
                this->setLayout(ly);
            }

            void setPossibleExercises(std::list<FreeFit::Data::Exercise> e)
            {
                for (auto w : workout_options)
                    w->setPossibleExercises(e);
            }

            WorkoutOption* getSelectedWorkout()
            {
                for (WorkoutOption* w : workout_options)
                    if (w->isChecked())
                        return w;
                return nullptr;
            }
        public slots:

            void accept() override
            {
                getSelectedWorkout()->generateWorkout();
                QDialog::accept();
            }

        private:
            QGridLayout* ly;
            WorkoutOption* all_exercises_workout;
            QIntValidator* int_validator;
            QLineEdit* number_of_lines;
            std::list<WorkoutOption*> workout_options;
            QDialogButtonBox* button_box;
        };
    }
}
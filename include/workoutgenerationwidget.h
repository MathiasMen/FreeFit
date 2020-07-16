#pragma once

#include <memory>

#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QIntValidator>
#include <QPushButton>
#include <QLabel>
#include <QStyle>

#include "include/exercise.h"
#include "include/workout.h"
#include "include/controls.h"

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

                next_page_button = new ControlButton("Workout",ControlButton::ForwardButton,ControlButton::Primary,this);
                connect(next_page_button, &QPushButton::clicked, this, &QDialog::accept);

                previous_page_button = new ControlButton("Exercises", ControlButton::BackwardButton,ControlButton::Primary,this);
                connect(previous_page_button, &QPushButton::clicked, this, &QDialog::reject);

                QGridLayout* controls_layout = new QGridLayout;
                QSpacerItem* horizontal_spacer = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
                controls_layout->addWidget(previous_page_button,0,0);
                controls_layout->addItem(horizontal_spacer,0,1);
                controls_layout->addWidget(next_page_button,0,2);

                ly->addLayout(controls_layout,2,0);
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
            ControlButton* next_page_button;
            ControlButton* previous_page_button;
        };
    }
}
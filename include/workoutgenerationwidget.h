#pragma once

#include <memory>

#include <QDialog>
#include <QGridLayout>
#include <QRadioButton>
#include <QLineEdit>

#include "include/exercise.h"
#include "include/workout.h"

namespace FreeFit
{
    namespace GUI
    {
        class WorkoutGenerationWidget : public QDialog
        {
        public:
            WorkoutGenerationWidget(QWidget* parent = nullptr) : QDialog(parent)
            {
                ly = new QGridLayout(this);

                number_of_lines = new QLineEdit("Enter number of rounds",this);
                ly->addWidget(number_of_lines,0,0);

                workout_types.push_back(std::make_shared<FreeFit::Data::AllExercisesWorkout>(std::list<FreeFit::Data::Exercise>()));
                all_exercises_workout = new QRadioButton("All Exercises",this);
                all_exercises_workout->setChecked(true);
                ly->addWidget(all_exercises_workout,1,0);

                this->setLayout(ly);
            }

            void setPossibleExercises(std::list<FreeFit::Data::Exercise> e)
            {
                for (auto w : workout_types)
                    w->setPossibleExercises(e);
            }
        private:
            QGridLayout* ly;
            QRadioButton* all_exercises_workout;
            QLineEdit* number_of_lines;
            std::list<std::shared_ptr<FreeFit::Data::WorkoutBase>> workout_types;
        };
    }
}
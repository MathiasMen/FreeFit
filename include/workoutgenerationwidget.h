#pragma once

#include <memory>
#include <functional>
#include <regex>

#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QIntValidator>
#include <QPushButton>
#include <QLabel>
#include <QStyle>

#include "include/exercise.h"
#include "include/workout.h"
#include "include/controls.h"
#include "include/materialdialog.h"
#include "include/materialtextfield.h"

#include <iostream>

namespace FreeFit
{
    namespace GUI
    {
        class WorkoutGenerationWidgetValidator;

        class WorkoutOptionBase : public QRadioButton
        {
            friend WorkoutGenerationWidgetValidator;
        public:
            WorkoutOptionBase(QString text = "", std::shared_ptr<FreeFit::Data::WorkoutBase> w = nullptr, QWidget* parent = nullptr) : QRadioButton(text,parent), workout_data(w)
            {

            }

            void setPossibleExercises(std::list<FreeFit::Data::Exercise> e)
            {
                workout_data->setPossibleExercises(e);
            }
        
            std::list<FreeFit::Data::Exercise> getExercisesPerRound(){return workout_data->getExercisesPerRound();}

            void setRounds(unsigned int r){workout_data->setRounds(r);}

            unsigned int getRounds(){return workout_data->getRounds();}

            void generateWorkout(){workout_data->generate();}

            std::shared_ptr<FreeFit::Data::WorkoutBase> getWorkout(){return workout_data;}

            QWidget* getOptionsWidget()
            {
                if (possible_options_widget != nullptr)
                    return possible_options_widget;
                else
                    return nullptr;
            }
        protected:
            std::shared_ptr<FreeFit::Data::WorkoutBase> workout_data;
            QWidget* possible_options_widget = nullptr;
        };

        class AllExercisesWorkoutOption : public WorkoutOptionBase
        {
        Q_OBJECT
        public:
            AllExercisesWorkoutOption(QString text = "", std::shared_ptr<FreeFit::Data::WorkoutBase> w = nullptr, QWidget* parent = nullptr) : WorkoutOptionBase(text,w,parent)
            {
                possible_options_widget = new QWidget;
                QVBoxLayout* ly = new QVBoxLayout(possible_options_widget);

                number_of_rounds = new MaterialTextField("Number of rounds",this);
                std::regex rounds_regex("[1-9]");
                auto func_rounds_regex = [rounds_regex](std::string s)->bool{return std::regex_match(s,rounds_regex);};
                number_of_rounds->setValidationFunction(func_rounds_regex);

                ly->addWidget(number_of_rounds);

                connect(number_of_rounds,&QLineEdit::textChanged,this,&AllExercisesWorkoutOption::numberOfRoundsChangedSlot);
            }
        signals:
            void numberOfRoundsChanged(int);
        private slots:
            void numberOfRoundsChangedSlot()
            {
                if (!number_of_rounds->text().isEmpty() && number_of_rounds->validateText())
                    emit numberOfRoundsChanged(std::stoi(number_of_rounds->text().toStdString()));
            }
        private:
            MaterialTextField* number_of_rounds;
        };

        class WorkoutGenerationWidget : public MaterialDialog
        {
            friend WorkoutGenerationWidgetValidator;
        Q_OBJECT
        public:
            WorkoutGenerationWidget(QWidget* parent = nullptr) : MaterialDialog(parent)
            {
                ly = new QGridLayout(this);

                number_of_rounds = new MaterialTextField("Number of rounds",this);
                int_validator = new QIntValidator(1,50,this);
                number_of_rounds->setValidator(int_validator);
                ly->addWidget(number_of_rounds,0,0);

                std::shared_ptr<FreeFit::Data::AllExercisesWorkout> w1 = std::make_shared<FreeFit::Data::AllExercisesWorkout>(std::list<FreeFit::Data::Exercise>());
                all_exercises_workout = new AllExercisesWorkoutOption("All Exercises",w1,this);
                ly->addWidget(all_exercises_workout,1,0);
                workout_options.push_back(all_exercises_workout);

                std::shared_ptr<FreeFit::Data::FilteredByMusclesWorkout> w2 = std::make_shared<FreeFit::Data::FilteredByMusclesWorkout>(std::list<FreeFit::Data::Exercise>());
                filtered_exercises_workout = new WorkoutOptionBase("Filtered by muscle groups",w2,this);
                ly->addWidget(filtered_exercises_workout,2,0);
                workout_options.push_back(filtered_exercises_workout);

                all_exercises_workout->setChecked(true);

                connect(number_of_rounds,&QLineEdit::textChanged,this,&WorkoutGenerationWidget::numberOfRoundsChanged);
                number_of_rounds->setText(QString::fromStdString(std::to_string(3)));

                next_page_button = new ControlButton("Workout",ControlButton::ForwardButton,ControlButton::Primary,this);
                connect(next_page_button, &QPushButton::clicked, this, &QDialog::accept);

                previous_page_button = new ControlButton("Exercises", ControlButton::BackwardButton,ControlButton::Primary,this);
                connect(previous_page_button, &QPushButton::clicked, this, &QDialog::reject);

                QSpacerItem* vertical_spacer = new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
                ly->addItem(vertical_spacer,2,0);

                QGridLayout* controls_layout = new QGridLayout;
                QSpacerItem* horizontal_spacer = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
                controls_layout->addWidget(previous_page_button,0,0);
                controls_layout->addItem(horizontal_spacer,0,1);
                controls_layout->addWidget(next_page_button,0,2);
                if (all_exercises_workout->getOptionsWidget() != nullptr)
                    ly->addWidget(all_exercises_workout->getOptionsWidget(),1,2);
                ly->addLayout(controls_layout,3,0);
                this->setLayout(ly);
            }

            void setPossibleExercises(std::list<FreeFit::Data::Exercise> e)
            {
                for (auto w : workout_options)
                    w->setPossibleExercises(e);
            }

            WorkoutOptionBase* getSelectedWorkout()
            {
                for (WorkoutOptionBase* w : workout_options)
                    if (w->isChecked())
                        return w;
                return nullptr;
            }
        public slots:
            void numberOfRoundsChanged()
            {
                if (!number_of_rounds->text().isEmpty())
                    getSelectedWorkout()->setRounds(std::stoi(number_of_rounds->text().toStdString()));
            }

            void accept() override
            {
                getSelectedWorkout()->generateWorkout();
                QDialog::accept();
            }

        private:
            QGridLayout* ly;
            WorkoutOptionBase* all_exercises_workout;
            WorkoutOptionBase* filtered_exercises_workout;
            QIntValidator* int_validator;
            MaterialTextField* number_of_rounds;
            std::list<WorkoutOptionBase*> workout_options;
            ControlButton* next_page_button;
            ControlButton* previous_page_button;
        };

        class WorkoutGenerationWidgetValidator : public QObject
        {
        public:
            WorkoutGenerationWidgetValidator(WorkoutGenerationWidget* t_w) : QObject(), w(t_w){}

            void setNumberOfRounds(unsigned int r){w->number_of_rounds->setText(QString::fromStdString(std::to_string(r)));}
        private:
            WorkoutGenerationWidget* w;
        };
    }
}
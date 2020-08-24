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
#include <QSizePolicy>

#include "include/exercise.h"
#include "include/exerciseeditor.h"
#include "include/workout.h"
#include "include/controls.h"
#include "include/hashtagbar.h"
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
        Q_OBJECT
        friend WorkoutGenerationWidgetValidator;
        public:
            WorkoutOptionBase(QString text = "", std::shared_ptr<FreeFit::Data::WorkoutBase> w = nullptr, QWidget* parent = nullptr) : QRadioButton(text,parent), workout_data(w)
            {
                possible_options_widget = new QWidget(this);
                QVBoxLayout* ly = new QVBoxLayout(possible_options_widget);

                number_of_rounds = new MaterialTextField("Number of rounds",possible_options_widget);
                std::regex rounds_regex("[1-9]");
                auto func_rounds_regex = [rounds_regex](std::string s)->bool{return std::regex_match(s,rounds_regex);};
                number_of_rounds->setValidationFunction(func_rounds_regex);

                ly->addWidget(number_of_rounds);

                connect(number_of_rounds,&QLineEdit::textChanged,this,&WorkoutOptionBase::numberOfRoundsChanged);
            }

            void setPossibleExercises(std::list<FreeFit::Data::Exercise> e)
            {
                workout_data->setPossibleExercises(e);
            }
        
            std::list<FreeFit::Data::Exercise> getExercisesPerRound(){return workout_data->getExercisesPerRound();}

            void setRounds(unsigned int r)
            {
                number_of_rounds->setText(QString::fromStdString(std::to_string(r)));
            }

            unsigned int getRounds(){return workout_data->getRounds();}

            void generateWorkout()
            {
                prepareWorkoutGeneration();
                workout_data->generate();
            }

            std::shared_ptr<FreeFit::Data::WorkoutBase> getWorkout(){return workout_data;}

            QWidget* getOptionsWidget()
            {
                if (possible_options_widget != nullptr)
                    return possible_options_widget;
                else
                    return nullptr;
            }

            virtual void prepareWorkoutGeneration() = 0;
        private slots:
            void numberOfRoundsChanged()
            {
                if (!number_of_rounds->text().isEmpty() && number_of_rounds->validateText())
                    workout_data->setRounds(std::stoi(number_of_rounds->text().toStdString()));
            }
        protected:
            std::shared_ptr<FreeFit::Data::WorkoutBase> workout_data;
            QWidget* possible_options_widget = nullptr;
            MaterialTextField* number_of_rounds;
        };

        class AllExercisesWorkoutOption : public WorkoutOptionBase
        {
        Q_OBJECT
        friend WorkoutGenerationWidgetValidator;
        public:
            AllExercisesWorkoutOption(QString text = "", std::shared_ptr<FreeFit::Data::WorkoutBase> w = nullptr, QWidget* parent = nullptr) : WorkoutOptionBase(text,w,parent)
            {
            }

            void prepareWorkoutGeneration(){}
        };

        class FilteredExercisesWorkoutOption : public WorkoutOptionBase
        {
        Q_OBJECT
        friend WorkoutGenerationWidgetValidator;
        public:
            FilteredExercisesWorkoutOption(QString text = "", std::shared_ptr<FreeFit::Data::FilteredByMusclesWorkout> w = nullptr, QWidget* parent = nullptr) : WorkoutOptionBase(text,w,parent)
            {
                muscle_areas = new HashtagBar(this);
                for (auto m : muscle_definitions.strings)
                    muscle_areas->addHashtag(m);
                possible_options_widget->layout()->addWidget(muscle_areas);
                specialized_workout = w;
            }

            void prepareWorkoutGeneration()
            {
                specialized_workout->setSelectedAreas(muscle_areas->getToggledStrings());
            }
        private:
            std::shared_ptr<FreeFit::Data::FilteredByMusclesWorkout> specialized_workout;
            MuscleGroups muscle_definitions;
            HashtagBar* muscle_areas;
        };

        class WorkoutGenerationWidget : public MaterialDialog
        {
            friend WorkoutGenerationWidgetValidator;
        Q_OBJECT
        public:
            WorkoutGenerationWidget(QWidget* parent = nullptr) : MaterialDialog(parent)
            {
                ly = new QGridLayout(this);

                options_canvas = new QStackedWidget(this);

                std::shared_ptr<FreeFit::Data::AllExercisesWorkout> w1 = std::make_shared<FreeFit::Data::AllExercisesWorkout>(std::list<FreeFit::Data::Exercise>());
                all_exercises_workout = new AllExercisesWorkoutOption("All Exercises",w1,this);
                ly->addWidget(all_exercises_workout,0,0);
                workout_options.push_back(all_exercises_workout);

                std::shared_ptr<FreeFit::Data::FilteredByMusclesWorkout> w2 = std::make_shared<FreeFit::Data::FilteredByMusclesWorkout>(std::list<FreeFit::Data::Exercise>());
                filtered_exercises_workout = new FilteredExercisesWorkoutOption("Filtered by muscle groups",w2,this);
                ly->addWidget(filtered_exercises_workout,1,0);
                workout_options.push_back(filtered_exercises_workout);

                all_exercises_workout->setChecked(true);

                all_exercises_workout->setRounds(3);
                filtered_exercises_workout->setRounds(3);

                options_canvas->addWidget(all_exercises_workout->getOptionsWidget());
                options_canvas->addWidget(filtered_exercises_workout->getOptionsWidget());
                ly->addWidget(options_canvas,1,2);

                connect(all_exercises_workout,&QRadioButton::toggled,this,&WorkoutGenerationWidget::optionChanged);
                connect(filtered_exercises_workout,&QRadioButton::toggled,this,&WorkoutGenerationWidget::optionChanged);

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

            void setNumberOfRounds(int n)
            {
                getSelectedWorkout()->setRounds(n);
            }

        public slots:
            void accept() override
            {
                getSelectedWorkout()->generateWorkout();
                QDialog::accept();
            }
        private slots:
            void optionChanged()
            {
                for (unsigned int i = 0; i < workout_options.size(); i++)
                    if (workout_options[i]->isChecked())
                        options_canvas->setCurrentIndex(i);
            }
        private:
            QGridLayout* ly;
            QStackedWidget* options_canvas;
            WorkoutOptionBase* all_exercises_workout;
            WorkoutOptionBase* filtered_exercises_workout;
            std::vector<WorkoutOptionBase*> workout_options;
            ControlButton* next_page_button;
            ControlButton* previous_page_button;
        };

        class WorkoutGenerationWidgetValidator : public QObject
        {
        public:
            WorkoutGenerationWidgetValidator(WorkoutGenerationWidget* t_w) : QObject(), w(t_w){}

            void setNumberOfRounds(unsigned int r){w->all_exercises_workout->number_of_rounds->setText(QString::fromStdString(std::to_string(r)));}
        private:
            WorkoutGenerationWidget* w;
        };
    }
}
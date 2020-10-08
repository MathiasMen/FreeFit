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
            WorkoutOptionBase(QString text = "", std::shared_ptr<FreeFit::Data::WorkoutBase> w = nullptr, QWidget* parent = nullptr);

            void setPossibleExercises(std::list<FreeFit::Data::Exercise> e);
        
            std::list<FreeFit::Data::Exercise> getExercisesPerRound(){return workout_data->getExercisesPerRound();}

            void setRounds(unsigned int r);

            unsigned int getRounds(){return workout_data->getRounds();}

            void generateWorkout();

            std::shared_ptr<FreeFit::Data::WorkoutBase> getWorkout(){return workout_data;}

            QWidget* getOptionsWidget();            
            virtual void prepareWorkoutGeneration() = 0;
            virtual void setColor(std::string c) = 0;
        private slots:
            void numberOfRoundsChanged();
            void numberOfExercisesPerRoundChanged();
            void timeOfExercisesChanged();
        protected:
            std::shared_ptr<FreeFit::Data::WorkoutBase> workout_data;
            QWidget* possible_options_widget = nullptr;
            QLabel* n_exercises_lbl;
            MaterialTextField* number_of_rounds;
            MaterialTextField* max_number_of_exercises;
            MaterialTextField* time_of_exercises;
        };

        class AllExercisesWorkoutOption : public WorkoutOptionBase
        {
        Q_OBJECT
        friend WorkoutGenerationWidgetValidator;
        public:
            AllExercisesWorkoutOption(QString text = "", std::shared_ptr<FreeFit::Data::WorkoutBase> w = nullptr, QWidget* parent = nullptr);

            void prepareWorkoutGeneration(){}

            void setColor(std::string c){}
        };

        class FilteredExercisesWorkoutOption : public WorkoutOptionBase
        {
        Q_OBJECT
        friend WorkoutGenerationWidgetValidator;
        public:
            FilteredExercisesWorkoutOption(QString text = "", std::shared_ptr<FreeFit::Data::FilteredByMusclesWorkout> w = nullptr, QWidget* parent = nullptr);

            void prepareWorkoutGeneration();

            void setColor(std::string c){muscle_areas->setColor(c);}
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
            WorkoutGenerationWidget(QWidget* parent = nullptr);

            void setColor(std::string c);

            void setPossibleExercises(std::list<FreeFit::Data::Exercise> e);

            WorkoutOptionBase* getSelectedWorkout();

            void setNumberOfRounds(int n);

        public slots:
            void accept() override;
        private slots:
            void optionChanged();
        private:
            QStackedWidget* options_canvas;
            QWidget* option_selection;
            QVBoxLayout* option_selection_ly;
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

            void setNumberOfRounds(unsigned int r){w->setNumberOfRounds(r);}

            WorkoutOptionBase* getWorkoutOption(unsigned int n){return w->workout_options[n];}

            void clickWorkoutOption(unsigned int n){w->workout_options[n]->click();}

            void toggleHashtag(std::string s)
            {
                FilteredExercisesWorkoutOption* opt = reinterpret_cast<FilteredExercisesWorkoutOption*>(w->filtered_exercises_workout);
                opt->muscle_areas->toggleHashtag(s);
            }
        private:
            WorkoutGenerationWidget* w;
        };
    }
}
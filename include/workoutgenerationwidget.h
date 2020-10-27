#pragma once

#include <memory>
#include <functional>
#include <regex>
#include <vector>

#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QLineEdit>
#include <QIntValidator>
#include <QPushButton>
#include <QLabel>
#include <QStyle>
#include <QSizePolicy>
#include <QComboBox>

#include "include/exercise.h"
#include "include/exerciseeditor.h"
#include "include/workout.h"
#include "include/controls.h"
#include "include/hashtagbar.h"
#include "include/materialdialog.h"
#include "include/materialtextfield.h"
#include "include/xmlreader.h"

namespace FreeFit
{
    namespace GUI
    {
        class WorkoutGenerationWidgetValidator;
        class WorkoutOptionBase;

        class ToggleContainer : public QWidget
        {
        Q_OBJECT
        public:
            explicit ToggleContainer(const QString & title = "", const int animationDuration = 300, QWidget *parent = 0);
            void setContent(WorkoutOptionBase* c);
            
            void setSelected(bool s, bool animation = true);
            bool isSelected();
        signals:
            void selected(ToggleContainer*,bool);
        private:
            void updateAnimationProperties(bool animation = true);

            QGridLayout main_layout;
            QToolButton toggle_button;
            QFrame header_line;
            QParallelAnimationGroup* toggle_animation;
            QPropertyAnimation* min_height_animation;
            QPropertyAnimation* max_height_animation;
            int animation_duration{300};
            WorkoutOptionBase* content;
        };

        class WorkoutOptionBase : public QWidget
        {
        Q_OBJECT
        friend WorkoutGenerationWidgetValidator;
        public:
            WorkoutOptionBase(std::shared_ptr<FreeFit::Data::WorkoutBase> w = nullptr, QWidget* parent = nullptr);

            virtual void setPossibleExercises(std::list<FreeFit::Data::Exercise> e);
        
            std::list<FreeFit::Data::Exercise> getExercisesPerRound(){return workout_data->getExercisesPerRound();}

            void setRounds(unsigned int r);

            unsigned int getRounds(){return workout_data->getRounds();}

            void generateWorkout();

            std::shared_ptr<FreeFit::Data::WorkoutBase> getWorkout(){return workout_data;}

            virtual void prepareWorkoutGeneration() = 0;
            virtual void setColor(std::string c) = 0;

            void click(){} // REMOVE!

            void setSelected(bool s){selected = s;}

            bool isSelected(){return selected;}

            QString getName(){return name;}
            void setName(QString n){name = n;}
        private slots:
            void numberOfRoundsChanged();
            void numberOfExercisesPerRoundChanged();
            void timeOfExercisesChanged();
        protected:
            QString name;
            bool selected = false;
            std::shared_ptr<FreeFit::Data::WorkoutBase> workout_data;
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
            AllExercisesWorkoutOption(std::shared_ptr<FreeFit::Data::WorkoutBase> w = nullptr, QWidget* parent = nullptr);

            void prepareWorkoutGeneration(){}

            void setColor(std::string c){}
        };

        class FilteredExercisesWorkoutOption : public WorkoutOptionBase
        {
        Q_OBJECT
        friend WorkoutGenerationWidgetValidator;
        public:
            FilteredExercisesWorkoutOption(std::shared_ptr<FreeFit::Data::FilteredByMusclesWorkout> w = nullptr, QWidget* parent = nullptr);

            void prepareWorkoutGeneration();

            void setColor(std::string c){muscle_areas->setColor(c);}
        private:
            std::shared_ptr<FreeFit::Data::FilteredByMusclesWorkout> specialized_workout;
            MuscleGroups muscle_definitions;
            HashtagBar* muscle_areas;
        };

        class CustomExercisesWorkoutOption : public WorkoutOptionBase
        {
        Q_OBJECT
        public:
            CustomExercisesWorkoutOption(std::shared_ptr<FreeFit::Data::CustomExercisesWorkout> w = nullptr, QWidget* parent = nullptr);
            void prepareWorkoutGeneration();
            void setColor(std::string c);
            void setPossibleExercises(std::list<FreeFit::Data::Exercise> e);
            void setSavedWorkouts(std::list<FreeFit::Data::CustomExercisesWorkout> l)
            {
                saved_workouts.clear();
                for (auto w : l)
                    saved_workouts.push_back(w);
                updateSavedWorkouts();
            }
        private slots:
            void selectSavedWorkout(int id_w);
            void changeCurrentWorkoutName(QString n);
            void updateExistingExercises();
        private:
            void addButtonClicked();
            void removeButtonClicked();
            void updateFilterLabel();
            void updateSavedWorkouts();
            void registerCustomWorkout(FreeFit::Data::CustomExercisesWorkout w);

            std::shared_ptr<FreeFit::Data::CustomExercisesWorkout> specialized_workout;
            std::vector<FreeFit::Data::CustomExercisesWorkout> saved_workouts;

            MaterialTextField* workout_name_ln;
            QComboBox* custom_workout_selection;
            QPushButton* save_workout_button;
            QPushButton* delete_workout_button;
            QWidget* filter_container;
            QHBoxLayout* filter_container_ly;
            MaterialTextField* exercises_filter_ln;
            QLabel* no_of_exercises_indicator;

            QWidget* lists_container;
            QGridLayout* lists_container_ly;
            QListWidget* existing_exercises_list;
            QPushButton* add_button;
            QPushButton* remove_button;
            QListWidget* selected_exercises_list;
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

            void setPathToSavedWorkouts(std::string path)
            {
                path_to_saved_workouts = path;
                updateCustomWorkouts();
            }
        public slots:
            void accept() override;
        private slots:
            void handleToggleContainerSelected(ToggleContainer* sender, bool selected);
        private:
            void updateCustomWorkouts();

            std::string path_to_saved_workouts;

            QStackedWidget* options_canvas;
            QWidget* option_selection;
            QVBoxLayout* option_selection_ly;
            WorkoutOptionBase* all_exercises_workout;
            WorkoutOptionBase* filtered_exercises_workout;
            WorkoutOptionBase* custom_exercises_workout;
            std::vector<WorkoutOptionBase*> workout_options;
            std::vector<ToggleContainer*> workout_containers;
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
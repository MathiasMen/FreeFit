#pragma once 

#include <list>
#include <iterator>

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QImage>
#include <QLabel>

#include "include/exercise.h"
#include "include/workout.h"
#include "include/datadefinitions.h"

namespace FreeFit
{
    namespace GUI
    {
        class ExerciseListWidgetItem : public QWidget
        {
            Q_OBJECT
        public:
            ExerciseListWidgetItem(QWidget*,FreeFit::Data::Exercise);
            void setRoundInformation(unsigned int, unsigned int);
            int getNameLength();
            void setNameLength(int);
        private:
            FreeFit::Data::Exercise e_dat;

            QGridLayout* ly;

            QLabel* lbl_name;
            QLabel* lbl_duration;
            QLabel* lbl_n_rounds;
            QLabel* lbl_trained_areas;
            QLabel* lbl_image;
            QImage  img_thumbnail;
        };

        class ExerciseListWidget : public QWidget
        {
            Q_OBJECT
        public:
            ExerciseListWidget(QWidget* parent);
            
            void generateWidgets(FreeFit::Data::WorkoutBase*);

            bool advanceCurrentExercise();
        private:
            void highlightExercise(FreeFit::GUI::ExerciseListWidgetItem*);

            void resetHighlightings();

            QVBoxLayout* ly;
            QVBoxLayout* sub_ly;
            QWidget* sub_widget;
            QScrollArea* scroll_area;
            std::list<ExerciseListWidgetItem*> exercise_widgets;
            std::list<FreeFit::GUI::ExerciseListWidgetItem*>::iterator current_exercise;
        signals:
            void exerciseClickedSignal(FreeFit::Data::Exercise d);
        public slots:
            void exerciseClickedSlot(FreeFit::Data::Exercise d);
            void exercisesUnclickableDemand();
            void exercisesClickableDemand();
        };
    }
}
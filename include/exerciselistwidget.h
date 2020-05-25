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
            ExerciseListWidget(QWidget* parent, std::list<FreeFit::Data::Exercise>);
        private:
            void generateListWidgets(std::list<FreeFit::Data::Exercise>);

            QVBoxLayout* ly;
            QVBoxLayout* sub_ly;
            QWidget* sub_widget;
            QScrollArea* scroll_area;
            std::list<ExerciseListWidgetItem*> exercise_widgets;
        signals:
            void exerciseClickedSignal(FreeFit::Data::Exercise d);
        public slots:
            void exerciseClickedSlot(FreeFit::Data::Exercise d);
            void exercisesUnclickableDemand();
            void exercisesClickableDemand();
        };
    }
}
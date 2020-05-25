#pragma once 

#include <list>
#include <iterator>

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>

#include "include/exercisedata.h"
#include "include/exercise.h"

namespace FreeFit
{
    namespace GUI
    {
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
            std::list<GUI::ExerciseDataWidget*> exercise_widgets;
        signals:
            void exerciseClickedSignal(FreeFit::Data::Exercise d);
        public slots:
            void exerciseClickedSlot(FreeFit::Data::Exercise d);
            void exercisesUnclickableDemand();
            void exercisesClickableDemand();
        };
    }
}
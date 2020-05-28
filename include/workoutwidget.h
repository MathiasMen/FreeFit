#pragma once

#include <QWidget>
#include <QGridLayout>

#include "include/workout.h"
#include "include/exerciselistwidget.h"
#include "include/exerciseviewer.h"

namespace FreeFit
{
    namespace GUI
    {
        class WorkoutWidget : public QWidget
        {
            Q_OBJECT
        public:
            WorkoutWidget(FreeFit::Data::WorkoutBase* t_w, QWidget* t_p = nullptr) : QWidget(t_p),w(t_w) 
            {
                ly = new QGridLayout(this);

                exercise_list = new FreeFit::GUI::ExerciseListWidget(this);
                exercise_view = new FreeFit::GUI::Exerciseviewer(this);

                exercise_list->generateWidgets(w);

                ly->addWidget(exercise_list,0,0,7,1);
                ly->addWidget(exercise_view,0,1,10,1);
                this->setLayout(ly);
            }
            void setWorkout(FreeFit::Data::WorkoutBase* t_w){w = t_w;}
        private:
            QGridLayout* ly;

            FreeFit::GUI::ExerciseListWidget* exercise_list;
            FreeFit::GUI::Exerciseviewer* exercise_view;
            //FreeFit::GUI::WorkoutWidgetControl* control; -> TODO

            FreeFit::Data::WorkoutBase* w;
        };
    }
}
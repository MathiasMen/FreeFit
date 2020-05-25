#include "include/exerciselistwidget.h"

#include <iostream>

FreeFit::GUI::ExerciseListWidget::ExerciseListWidget(QWidget* parent, std::list<FreeFit::Data::Exercise> t_lst)
    : QWidget(parent)
{
    scroll_area = new QScrollArea(this);

    generateListWidgets(t_lst);

    ly = new QVBoxLayout(this);
    ly->addWidget(scroll_area);
    this->setLayout(ly);
}

void FreeFit::GUI::ExerciseListWidget::generateListWidgets(std::list<FreeFit::Data::Exercise> lst)
{
/*
    if (sub_widget != nullptr)
        delete sub_widget;
*/
    sub_widget = new QWidget(this);
    sub_ly = new QVBoxLayout(sub_widget);
    for (auto e : lst)
    {
        //FreeFit::GUI::ExerciseDataWidget* w = new FreeFit::GUI::ExerciseDataWidget(sub_widget,e);
        //QObject::connect(w,SIGNAL(exerciseClicked(FreeFit::Data::Exercise)),this,SLOT(exerciseClickedSlot(FreeFit::Data::Exercise)));
        //sub_ly->addWidget(w);
        //exercise_widgets.push_back(w);
//        w->show();
    }
    sub_widget->setLayout(sub_ly);
    scroll_area->setWidget(sub_widget);
}

void FreeFit::GUI::ExerciseListWidget::exerciseClickedSlot(FreeFit::Data::Exercise d)
{
    emit exerciseClickedSignal(d);
}

void FreeFit::GUI::ExerciseListWidget::exercisesUnclickableDemand()
{
//    for (auto it : exercise_widgets)
//        it->setActive(false);
}

void FreeFit::GUI::ExerciseListWidget::exercisesClickableDemand()
{
//    for (auto it : exercise_widgets)
//        it->setActive(true);
}

#include "include/exerciselistwidget.h"

#include <iostream>

FreeFit::GUI::ExerciseListWidgetItem::ExerciseListWidgetItem(QWidget* parent, FreeFit::Data::Exercise t_e) : QWidget(parent), e_dat(t_e)
{
    ly = new QGridLayout(this);
    lbl_name = new QLabel(QString::fromStdString(e_dat.getName()),this);
    lbl_duration = new QLabel("30",this);
    lbl_n_rounds = new QLabel("3",this);
    std::string areas = "";
    for (auto a : e_dat.getTrainedMuscles())
        areas.append("#" + FreeFit::Data::muscleGroupToString(a) + " ");
    lbl_trained_areas = new QLabel(QString::fromStdString(areas),this);
/*
    QImage  img_thumbnail;
    lbl_image
*/
    
    ly->addWidget(lbl_name,0,0);
    ly->addWidget(lbl_duration,0,1);
    ly->addWidget(lbl_n_rounds,0,2);
    ly->addWidget(lbl_trained_areas,1,0,1,3);
    this->setLayout(ly);
} 

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
        FreeFit::GUI::ExerciseListWidgetItem* i = new FreeFit::GUI::ExerciseListWidgetItem(sub_widget,e);
        //QObject::connect(w,SIGNAL(exerciseClicked(FreeFit::Data::Exercise)),this,SLOT(exerciseClickedSlot(FreeFit::Data::Exercise)));
        sub_ly->addWidget(i);
        exercise_widgets.push_back(i);
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

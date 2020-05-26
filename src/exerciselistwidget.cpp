#include "include/exerciselistwidget.h"

#include <iostream>

FreeFit::GUI::ExerciseListWidgetItem::ExerciseListWidgetItem(QWidget* parent,FreeFit::Data::Exercise t_e) : QWidget(parent), e_dat(t_e)
{
    ly = new QGridLayout(this);
    lbl_name = new QLabel(QString::fromStdString(e_dat.getName()),this);
    lbl_duration = new QLabel("30",this);
    lbl_n_rounds = new QLabel("-/-",this);
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

void FreeFit::GUI::ExerciseListWidgetItem::setRoundInformation(unsigned int r, unsigned int r_total)
{
    lbl_n_rounds->setText(QString("%1 / %2").arg(r).arg(r_total));
}


FreeFit::GUI::ExerciseListWidget::ExerciseListWidget(QWidget* parent)
    : QWidget(parent)
{
    scroll_area = new QScrollArea(this);

    ly = new QVBoxLayout(this);
    ly->addWidget(scroll_area);
    this->setLayout(ly);
}

void FreeFit::GUI::ExerciseListWidget::generateWidgets(FreeFit::Data::WorkoutBase* w)
{
    sub_widget = new QWidget(this);
    sub_ly = new QVBoxLayout(sub_widget);
    unsigned int rounds_total = w->getRounds();

    for (unsigned int r = 1; r <= rounds_total; r++)
        for (auto e : w->getExercisesPerRound())
        {
            FreeFit::GUI::ExerciseListWidgetItem* i = new FreeFit::GUI::ExerciseListWidgetItem(sub_widget,e);
            i->setRoundInformation(r,rounds_total);
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

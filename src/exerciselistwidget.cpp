#include "include/exerciselistwidget.h"

GUI::ExerciseListWidget::ExerciseListWidget(QWidget* parent, std::list<Data::ExerciseData>* t_lst)
    : QWidget(parent)
{
    scroll_area = new QScrollArea(this);

    generateListWidgets(t_lst);

    ly = new QVBoxLayout(this);
    ly->addWidget(scroll_area);
    this->setLayout(ly);
}

std::map<std::string, int> GUI::ExerciseListWidget::count_el_in_list(const std::vector<std::string>& v)
{
    std::map<std::string, int> result;
    for (auto const& s : v) {
        ++result[s];
    }
    return result;
}

void GUI::ExerciseListWidget::generateListWidgets(std::list<Data::ExerciseData>* lst)
{
    if (sub_widget != nullptr)
        delete sub_widget;

    sub_widget = new QWidget(this);
    sub_ly = new QVBoxLayout(sub_widget);
    for (auto it : *lst)
    {
        GUI::ExerciseDataWidget* w = new GUI::ExerciseDataWidget(sub_widget,it);
        QObject::connect(w,SIGNAL(exerciseClicked(Data::ExerciseData)),this,SLOT(exerciseClickedSlot(Data::ExerciseData)));
        sub_ly->addWidget(w);
        exercise_widgets.push_back(w);
        w->show();
    }
    sub_widget->setLayout(sub_ly);
    scroll_area->setWidget(sub_widget);
}

void GUI::ExerciseListWidget::exerciseClickedSlot(Data::ExerciseData d)
{
    emit exerciseClickedSignal(d);
}

void GUI::ExerciseListWidget::exercisesUnclickableDemand()
{
    for (auto it : exercise_widgets)
        it->setActive(false);
}

void GUI::ExerciseListWidget::exercisesClickableDemand()
{
    for (auto it : exercise_widgets)
        it->setActive(true);
}

#ifndef EXERCISELISTWIDGET_H
#define EXERCISELISTWIDGET_H

#include <list>
#include <iterator>
#include <exercisedata.h>

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>

namespace GUI{
    class ExerciseListWidget : public QWidget
    {
        Q_OBJECT
    public:
        ExerciseListWidget(QWidget* parent, std::list<Data::ExerciseData>*);
        void generateListWidgets(std::list<Data::ExerciseData>*);
    private:
        QVBoxLayout* ly;
        QVBoxLayout* sub_ly;
        QWidget* sub_widget;
        QScrollArea* scroll_area;
        std::map<std::string, int> count_el_in_list(const std::vector<std::string>&);
        std::list<GUI::ExerciseDataWidget*> exercise_widgets;
    signals:
        void exerciseClickedSignal(Data::ExerciseData d);
    public slots:
        void exerciseClickedSlot(Data::ExerciseData d);
        void exercisesUnclickableDemand();
        void exercisesClickableDemand();
    };
}
#endif // EXERCISELISTWIDGET_H

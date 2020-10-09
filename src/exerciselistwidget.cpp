#include "include/exerciselistwidget.h"

FreeFit::GUI::ExerciseListWidgetItem::ExerciseListWidgetItem(QWidget* parent,FreeFit::Data::Exercise t_e) : QWidget(parent), e_dat(t_e), color("#ff0000"), is_bold(false)
{
    ly = new QGridLayout(this);
    lbl_name = new QLabel(QString::fromStdString(e_dat.getName()),this);
    lbl_duration = new QLabel(QDateTime::fromTime_t(e_dat.getLength()).toUTC().toString("mm:ss"),this);
    
    trained_areas = new HashtagBar(this);
    for (auto a : e_dat.getTrainedMuscles())
        trained_areas->addHashtag(FreeFit::Data::muscleGroupToString(a));
    trained_areas->setLabelsSelectable(false);

    lbl_image = new QLabel("",this);
    if(e_dat.getThumbnailPath() != "")
    {
        img_thumbnail = QImage(QString::fromStdString(e_dat.getThumbnailPath()));
        const unsigned int img_scaling = 8;
        img_thumbnail = img_thumbnail.scaled(16*img_scaling,9*img_scaling);
        lbl_image->setPixmap(QPixmap::fromImage(img_thumbnail));
        lbl_image->adjustSize();
    }

    ly->addWidget(lbl_image,0,0,2,1);
    ly->addWidget(lbl_name,0,1);
    ly->addWidget(lbl_duration,0,2);
    ly->addWidget(trained_areas,1,1,1,3);
    this->setLayout(ly);
} 

void FreeFit::GUI::ExerciseListWidgetItem::updateCSS()
{
    QString css_string = QString::fromStdString("color:" + color + ";");
    if (is_bold)
        css_string += " font-weight:bold;";
    lbl_name->setStyleSheet(css_string);
    lbl_duration->setStyleSheet(css_string);
}

void FreeFit::GUI::ExerciseListWidgetItem::setColor(std::string c)
{
    color = c;
    trained_areas->setColor(c);
    updateCSS();
}

void FreeFit::GUI::ExerciseListWidgetItem::setBold(bool b)
{
    is_bold = b;
    updateCSS();
}

int FreeFit::GUI::ExerciseListWidgetItem::getNameLength()
{
    return lbl_name->width();
}

void FreeFit::GUI::ExerciseListWidgetItem::setNameLength(int l)
{
    lbl_name->setMinimumWidth(l);
}

FreeFit::GUI::ExerciseListWidget::ExerciseListWidget(QWidget* parent)
    : QWidget(parent),color("#ff0000")
{
    scroll_area = new QScrollArea(this);

    ly = new QVBoxLayout(this);
    ly->addWidget(scroll_area);
    this->setLayout(ly);
}

void FreeFit::GUI::ExerciseListWidget::generateWidgets(FreeFit::Data::WorkoutBase* w)
{
    sub_widget = new QWidget(this);
    sub_ly = new QGridLayout(sub_widget);
    unsigned int rounds_total = w->getRounds();

    unsigned int row_counter = 0;
    unsigned int n_exercises = w->getExercisesPerRound().size();
    for (unsigned int r = 1; r <= rounds_total; r++)
    {
        for (auto e : w->getExercisesPerRound())
        {
            FreeFit::GUI::ExerciseListWidgetItem* i = new FreeFit::GUI::ExerciseListWidgetItem(sub_widget,e);
            sub_ly->addWidget(i,row_counter,1);
            exercise_widgets.push_back(i);
            row_counter += 1;
        }
        QLabel* round_label = new QLabel("Round " + QString::number(r));
        QGraphicsScene* scene = new QGraphicsScene(sub_widget);
        scene->addWidget(round_label);
        QGraphicsView* view = new QGraphicsView(scene,this);
        view->rotate(-90);
        sub_ly->addWidget(view,(r-1)*n_exercises,0,n_exercises,1);
    }
    int max_width = 0;
    for (auto widget : exercise_widgets)
        if (max_width < widget->getNameLength())
            max_width = widget->getNameLength();

    for (auto widget : exercise_widgets)
        widget->setNameLength(max_width);

    sub_widget->setLayout(sub_ly);
    scroll_area->setWidget(sub_widget);

    current_exercise  = exercise_widgets.begin();
    highlightExercise(&(**current_exercise));
}

bool FreeFit::GUI::ExerciseListWidget::advanceCurrentExercise()
{
    resetHighlightings();
    std::advance(current_exercise,1);

    if (current_exercise == exercise_widgets.end())
        return false;

    highlightExercise(&(**current_exercise));
    return true;
}

int FreeFit::GUI::ExerciseListWidget::getLengthOfCurrentExercise()
{
    return (**current_exercise).getExerciseLength();
}

std::string FreeFit::GUI::ExerciseListWidget::getVideoPathOfCurrentExercise()
{
    return (**current_exercise).getVideoPath();
}

void FreeFit::GUI::ExerciseListWidget::highlightExercise(FreeFit::GUI::ExerciseListWidgetItem* e)
{
    e->setBold(true);
}

void FreeFit::GUI::ExerciseListWidget::setColor(std::string c)
{
    for (auto e : exercise_widgets)
        e->setColor(c);
}

void FreeFit::GUI::ExerciseListWidget::resetHighlightings()
{
    for (auto e : exercise_widgets)
        e->setBold(false);
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

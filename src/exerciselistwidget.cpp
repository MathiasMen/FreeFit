#include "include/exerciselistwidget.h"

FreeFit::GUI::ExerciseListWidgetItem::ExerciseListWidgetItem(QWidget* parent,FreeFit::Data::Exercise t_e) : QWidget(parent), e_dat(t_e)
{
    ly = new QGridLayout(this);
    lbl_name = new QLabel(QString::fromStdString(e_dat.getName()),this);
    lbl_duration = new QLabel(QDateTime::fromTime_t(e_dat.getLength()).toUTC().toString("mm:ss"),this);
    lbl_n_rounds = new QLabel("-/-",this);
    std::string areas = "";
    for (auto a : e_dat.getTrainedMuscles())
        areas.append("#" + FreeFit::Data::muscleGroupToString(a) + " ");
    lbl_trained_areas = new QLabel(QString::fromStdString(areas),this);

    lbl_image = new QLabel("",this);
    if(e_dat.getThumbnailPath() != "")
    {
        img_thumbnail = QImage(QString::fromStdString(e_dat.getThumbnailPath()));
        img_thumbnail = img_thumbnail.scaled(50,50);
        lbl_image->setPixmap(QPixmap::fromImage(img_thumbnail));
        lbl_image->adjustSize();
    }

    ly->addWidget(lbl_image,0,0,2,1);
    ly->addWidget(lbl_name,0,1);
    ly->addWidget(lbl_duration,0,2);
    ly->addWidget(lbl_n_rounds,0,3);
    ly->addWidget(lbl_trained_areas,1,1,1,3);
    this->setLayout(ly);
} 

void FreeFit::GUI::ExerciseListWidgetItem::setRoundInformation(unsigned int r, unsigned int r_total)
{
    lbl_n_rounds->setText(QString("%1 / %2").arg(r).arg(r_total));
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
    sub_ly = new QGridLayout(sub_widget);
    unsigned int rounds_total = w->getRounds();

    unsigned int row_counter = 0;
    unsigned int n_exercises = w->getExercisesPerRound().size();
    for (unsigned int r = 1; r <= rounds_total; r++)
    {
        for (auto e : w->getExercisesPerRound())
        {
            FreeFit::GUI::ExerciseListWidgetItem* i = new FreeFit::GUI::ExerciseListWidgetItem(sub_widget,e);
            i->setRoundInformation(r,rounds_total);
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
    e->setStyleSheet("background-color:red;");
}

void FreeFit::GUI::ExerciseListWidget::resetHighlightings()
{
    for (auto e : exercise_widgets)
        e->setStyleSheet("");
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

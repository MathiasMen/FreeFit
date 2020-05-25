#include "exercisedata.h"

QString Data::parseBodyPartsToString(std::list<Data::BodyPart> l)
{
    QString res;
    for (auto b : l)
        res.append(bodypart_to_str[b] + QString(", "));

    int last_comma_pos = res.lastIndexOf(QChar(','));
    res = res.left(last_comma_pos);
    return res;
}

Data::ExerciseData::ExerciseData(QString t_n, int t_reps_per_round, int t_secs_per_round, int t_rounds, std::list<BodyPart> t_a, QString t_yt_vid, QString t_thbn, int t_current_round)
    : name(t_n),
      reps_per_round(t_reps_per_round),
      secs_per_round(t_secs_per_round),
      rounds(t_rounds),
      current_round(t_current_round),
      trained_areas(t_a),
      youtube_video_path(t_yt_vid),
      thumbnail_path(t_thbn)
{
}

Data::ExerciseData::Type Data::ExerciseData::get_type()
{
    if (reps_per_round == -1 && secs_per_round == -1)
        return Type::Default;
    else if (reps_per_round == -1)
        return Type::TimeBased;
    else if (secs_per_round == -1)
        return Type::RepBased;
    else
        return Type::Default;
}

void GUI::ExerciseDataWidget::changeFontSize(QLabel* l, int s, bool b)
{
    QFont f = l->font();
    f.setPointSize(s);
    f.setBold(b);
    l->setFont(f);
}

void GUI::ExerciseDataWidget::init_layout()
{
    ly = new QGridLayout(this);

    lbl_thumbnail = new QLabel(this);
    lbl_name = new QLabel(this);
    lbl_reps_per_round = new QLabel(this);
    lbl_seconds_per_round = new QLabel(this);
    lbl_rounds_total = new QLabel(this);
    lbl_trained_areas = new QLabel(this);

    //resetTrainedAreasLabel();

    const int font_size = 13;

    changeFontSize(lbl_name, font_size);
    changeFontSize(lbl_reps_per_round, font_size);
    changeFontSize(lbl_seconds_per_round, font_size);
    changeFontSize(lbl_rounds_total, font_size);

    ly->addWidget(lbl_thumbnail,0,0,2,1,Qt::AlignLeft);
    ly->addWidget(lbl_name,0,1,1,3,Qt::AlignLeft);

    QStackedWidget* wdgt_amount_per_round = new QStackedWidget(this);
    wdgt_amount_per_round->addWidget(lbl_seconds_per_round);
    wdgt_amount_per_round->addWidget(lbl_reps_per_round);
    if(this->is_time_based())
        wdgt_amount_per_round->setCurrentIndex(0);
    else
        wdgt_amount_per_round->setCurrentIndex(1);

    ly->addWidget(wdgt_amount_per_round,1,1,Qt::AlignLeft);
    ly->addWidget(lbl_rounds_total,1,2,Qt::AlignHCenter);
    ly->addWidget(lbl_trained_areas,1,3,Qt::AlignRight);

    this->setLayout(ly);
}

void GUI::ExerciseDataWidget::resetTrainedAreasLabel()
{
    lbl_trained_areas->setText(Data::parseBodyPartsToString(this->dat.trained_areas));
}

void GUI::ExerciseDataWidget::reset_gui_elements()
{
    lbl_name->setText(dat.name);

    if(this->is_time_based())
        lbl_seconds_per_round->setText(QString("%1 sec.").arg(dat.secs_per_round));
    else
        lbl_reps_per_round->setText(QString("%1 rep.").arg(dat.reps_per_round));

    lbl_rounds_total->setText(QString("%1 / %2").arg(dat.current_round).arg(dat.rounds));

    if(!img_thumbnail.isNull())
    {
        lbl_thumbnail->setPixmap(QPixmap::fromImage(img_thumbnail));
        lbl_thumbnail->adjustSize();
    }
}

void GUI::ExerciseDataWidget::init()
{
    if(dat.thumbnail_path != "")
    {
        img_thumbnail = QImage(dat.thumbnail_path);
        img_thumbnail = img_thumbnail.scaled(50,50);
    }

    init_layout();
    reset_gui_elements();
    is_active = true;
}

void GUI::ExerciseDataWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (is_active)
        emit exerciseClicked(dat);
}

bool GUI::ExerciseDataWidget::is_time_based()
{
    if(dat.secs_per_round > 0 && dat.reps_per_round <= 0)
        return true;
    else if (dat.secs_per_round <= 0 && dat.reps_per_round > 0)
        return false;
    else
        std::cout << "Something bad happened!" << std::endl;
        return false;
}

GUI::ExerciseDataWidget::ExerciseDataWidget(QWidget* parent, Data::ExerciseData d) : QWidget(parent), dat(d)
{
    init();
}

void GUI::ExerciseDataWidget::setActive(bool a)
{
    is_active = a;
}

#ifndef EXERCISEDATA_H
#define EXERCISEDATA_H

#include <list>
#include <set>
#include <iostream>

#include <QString>
#include <QWidget>
#include <QLayout>
#include <QGridLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QFont>
#include <QImage>
#include <QPixmap>
#include <QTimer>


namespace Data{

    enum BodyPart {Calves = 1, Thighs = 2, LowerCore = 11, UpperCore = 12, SideCore = 13, Biceps = 21, Triceps = 22, LowerBack = 31, UpperBack = 32};
    static std::map<QString,BodyPart> str_to_bodypart = {{"Calves",BodyPart::Calves},
                                                             {"Thighs",BodyPart::Thighs},
                                                             {"LowerCore",BodyPart::LowerCore},
                                                             {"UpperCore",BodyPart::UpperCore},
                                                             {"SideCore",BodyPart::SideCore},
                                                             {"Biceps",BodyPart::Biceps},
                                                             {"Triceps",BodyPart::Triceps},
                                                             {"LowerBack",BodyPart::LowerBack},
                                                             {"UpperBack",BodyPart::UpperBack}};

    static std::map<BodyPart,QString> bodypart_to_str = {       {BodyPart::Calves,"Calves"},
                                                                {BodyPart::Thighs,"Thighs"},
                                                                {BodyPart::LowerCore,"LowerCore"},
                                                                {BodyPart::UpperCore,"UpperCore"},
                                                                {BodyPart::SideCore,"SideCore"},
                                                                {BodyPart::Biceps,"Biceps"},
                                                                {BodyPart::Triceps,"Triceps"},
                                                                {BodyPart::LowerBack,"LowerBack"},
                                                                {BodyPart::UpperBack,"UpperBack"}};

    std::list<BodyPart> parseStringToBodyParts(QString);
    QString parseBodyPartsToString(std::list<BodyPart>);

    class ExerciseData
    {
    public:
        enum Type {Default, TimeBased, RepBased};

        ExerciseData(QString = "", int = -1, int = -1, int = -1, std::list<BodyPart> = std::list<BodyPart>(), QString = "", QString = "", int = -1);

        QString name;
        int reps_per_round;
        int secs_per_round;
        int rounds;
        int current_round;
        std::list<BodyPart> trained_areas;
        QString youtube_video_path;
        QString thumbnail_path;

        Type get_type();
    };
}

namespace GUI{
    class ExerciseDataWidget : public QWidget
    {
        Q_OBJECT
    public:
        ExerciseDataWidget(QWidget*, Data::ExerciseData);

        void setActive(bool);

        const Data::ExerciseData* getData();
    private:
        const Data::ExerciseData dat;

        QGridLayout* ly;

        QLabel* lbl_name;
        QLabel* lbl_reps_per_round;
        QLabel* lbl_seconds_per_round;
        QLabel* lbl_rounds_total;
        QImage  img_thumbnail;
        QLabel* lbl_trained_areas;
        QLabel* lbl_thumbnail;

        void changeFontSize(QLabel*, int, bool = false);
        void resetTrainedAreasLabel();
        void init_layout();
        void init();

        void mouseDoubleClickEvent(QMouseEvent *event) override;

        bool is_time_based();

        bool is_active;
    signals:
        void exerciseClicked(Data::ExerciseData dat);
    public slots:
        void reset_gui_elements();
    };
}
#endif // EXERCISEDATA_H

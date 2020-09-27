#pragma once 

#include <list>
#include <iterator>

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QImage>
#include <QLabel>
#include <QDateTime>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "include/exercise.h"
#include "include/workout.h"
#include "include/datadefinitions.h"
#include "include/exerciseeditor.h"

namespace FreeFit
{
    namespace GUI
    {
        class ExerciseListWidgetItem : public QWidget
        {
            Q_OBJECT
        public:
            ExerciseListWidgetItem(QWidget*,FreeFit::Data::Exercise);
            int getNameLength();
            void setNameLength(int);
            void setExerciseLength(int l){e_dat.setLength(l);}
            int getExerciseLength(){return e_dat.getLength();}
            std::string getVideoPath(){return e_dat.getVideoPath();}
            void setColor(std::string c);
            void setBold(bool b);
        private:
            void updateCSS();
            
            FreeFit::Data::Exercise e_dat;

            std::string color;
            bool is_bold;

            QGridLayout* ly;

            QLabel* lbl_name;
            QLabel* lbl_duration;
            QWidget* trained_areas;
            QLabel* lbl_image;
            QImage  img_thumbnail;
        };

        class ExerciseListWidget : public QWidget
        {
            Q_OBJECT
        public:
            ExerciseListWidget(QWidget* parent);
            
            void generateWidgets(FreeFit::Data::WorkoutBase*);

            bool advanceCurrentExercise();

            int getLengthOfCurrentExercise();

            int getNumberOfExerciseWidgets(){return exercise_widgets.size();}
            
            std::string getVideoPathOfCurrentExercise();

            void setColor(std::string c);
        private:
            void highlightExercise(FreeFit::GUI::ExerciseListWidgetItem*);

            void resetHighlightings();

            QVBoxLayout* ly;
            QGridLayout* sub_ly;
            QWidget* sub_widget;
            QScrollArea* scroll_area;
            std::list<ExerciseListWidgetItem*> exercise_widgets;
            std::list<FreeFit::GUI::ExerciseListWidgetItem*>::iterator current_exercise;

            std::string color;
        signals:
            void exerciseClickedSignal(FreeFit::Data::Exercise d);
        public slots:
            void exerciseClickedSlot(FreeFit::Data::Exercise d);
            void exercisesUnclickableDemand();
            void exercisesClickableDemand();
        };
    }
}
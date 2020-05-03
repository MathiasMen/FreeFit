#pragma once

#include <QApplication>
#include <QString>
#include <QLabel>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QScrollArea>
#include <QGroupBox>
#include <QStyleOption>
#include <QPainter>
#include <QStyle>

#include <iostream>
#include <vector>

#include "include/newexercisedemand.h"

namespace FreeFit
{
    namespace GUI
    {
        class ClickableLabel : public QLabel
        {
            Q_OBJECT
        public:
            ClickableLabel(QString text,QWidget* parent):QLabel(text,parent){}

        protected:
            void mousePressEvent(QMouseEvent* ev) override
            {
                emit labelClicked();
                QLabel::mousePressEvent(ev);
            }

        signals:
            void labelClicked();
        };

        class WriteableLine : public QLineEdit
        {
            Q_OBJECT
        public:
            WriteableLine(QString text, QWidget* parent):QLineEdit(parent){this->setText(text);}
        protected:
            void focusOutEvent(QFocusEvent* ev)
            {
                emit textMessageBecauseFocusLost(this->text());
                QLineEdit::focusOutEvent(ev);
            }
        signals:
            void textMessageBecauseFocusLost(QString t);
        };

        class EditableLine : public QStackedWidget
        {
            Q_OBJECT
        public:
            EditableLine(QString text,QWidget* parent):QStackedWidget(parent)
            {
                this->setFocusPolicy(Qt::StrongFocus);
                l = new ClickableLabel(text,this);
                le = new WriteableLine(text,this);
                l->setStyleSheet("background-color:green;");
                le->setStyleSheet("background-color:red;");
                this->addWidget(l);
                this->addWidget(le);
                connect(l,&ClickableLabel::labelClicked,this,&EditableLine::showLineEdit);
                connect(le,&WriteableLine::textMessageBecauseFocusLost,this,&EditableLine::showLabelAndSetText);
            }
        public slots:
            void showLineEdit()
            {
                this->setCurrentWidget(le);
            }

            void showLabelAndSetText(QString t)
            {
                this->setCurrentWidget(l);
                l->setText(t);
            }
        private:
            ClickableLabel* l;
            WriteableLine* le;
        };
        
        class ExerciseItem : public QWidget
        {
            Q_OBJECT
        public:
            ExerciseItem(QWidget* parent):QWidget(parent)
            {
                this->setFocusPolicy(Qt::ClickFocus);
                this->setStyleSheet("background-color:blue;");

                ly = new QGridLayout(this);
                
                name_label = new QLabel("Name:",this);
                url_label = new QLabel("URL:",this);
                start_time_label = new QLabel("Start Time:",this);
                stop_time_label = new QLabel("Stop Time:",this);

                name        = new EditableLine("...",this);
                url         = new EditableLine("...",this);
                start_time  = new EditableLine("...",this);
                stop_time   = new EditableLine("...",this);

                int row_counter = -1;
                ly->addWidget(name_label,++row_counter,0);
                ly->addWidget(url_label,++row_counter,0);
                ly->addWidget(start_time_label,++row_counter,0);
                ly->addWidget(stop_time_label,++row_counter,0);

                row_counter = -1;
                ly->addWidget(name,++row_counter,1);
                ly->addWidget(url,++row_counter,1);
                ly->addWidget(start_time,++row_counter,1);
                ly->addWidget(stop_time,++row_counter,1);
            }

        private:

            void paintEvent(QPaintEvent* e)
            {
                QStyleOption opt;
                opt.init(this);
                QPainter p(this);
                style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
            }

            QGridLayout* ly;

            QLabel* name_label;
            QLabel* url_label;
            QLabel* start_time_label;
            QLabel* stop_time_label;

            EditableLine* name;
            EditableLine* url;
            EditableLine* start_time;
            EditableLine* stop_time;
        };

        class ExerciseEditor : public QDialog
        {
            Q_OBJECT
        public:
            ExerciseEditor()
            {                
                ly = new QGridLayout(this);

                add_button = new QPushButton("Add Exercise",this);
                download_all_button = new QPushButton("Download All",this);
                connect(add_button,&QPushButton::clicked,this,&ExerciseEditor::addExercise);
                connect(download_all_button,&QPushButton::clicked,this,&ExerciseEditor::downloadAllExercises);

                exercise_area = new QGroupBox("Exercises to download", this);
                exercise_area_ly = new QVBoxLayout(exercise_area);
                exercise_area->setLayout(exercise_area_ly);

                ExerciseItem* e = new ExerciseItem(exercise_area);
                exercise_area_ly->addWidget(e);

                scroll_area = new QScrollArea(this);
                scroll_area->setWidget(exercise_area);
                scroll_area->setWidgetResizable(true);

                ly->addWidget(add_button,0,0,1,1);
                ly->addWidget(download_all_button,0,3,1,1);
                ly->addWidget(scroll_area,1,0,4,4);
                this->setLayout(ly);
            };
        private:
            QPushButton* add_button;
            QPushButton* download_all_button;
            QScrollArea* scroll_area;
            QGroupBox* exercise_area;
            QVBoxLayout* exercise_area_ly;
            QGridLayout* ly;
        private slots:
            void addExercise()
            {
                ExerciseItem* e = new ExerciseItem(exercise_area);
                exercise_area_ly->addWidget(e);
            }

            void downloadAllExercises()
            {
                std::cout << "Stub: Download all was started!" << std::endl;
            }
        };
    }
}

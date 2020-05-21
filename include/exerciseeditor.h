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
#include <QListWidget>
#include <QDialogButtonBox>

#include <iostream>
#include <set>
#include <functional>
#include <regex>
#include <memory>

#include "include/downloadexercisedemand.h"
#include "include/downloadexercisedemandhandler.h"
#include "include/profile.h"
#include "include/exercise.h"
#include "include/datadefinitions.h"
#include "include/xmlreader.h"

namespace FreeFit
{
    namespace GUI
    {
        class ExerciseEditorValidator;

        class ToggleableLabel : public QLabel
        {
            Q_OBJECT
        friend ExerciseEditorValidator;
        public:
            ToggleableLabel(QString text, QWidget* parent):QLabel(text,parent){}
            bool isToggled(){return toggled;}
            void click(){clicked();};

        protected:
            void mousePressEvent(QMouseEvent* ev) override;
            void clicked();

        private:
            bool toggled = false;
        };

        class ClickableLabel : public QLabel
        {
            Q_OBJECT
        friend ExerciseEditorValidator;
        public:
            ClickableLabel(QString text,QWidget* parent):QLabel(text,parent){}

        protected:
            void mousePressEvent(QMouseEvent* ev) override;

        signals:
            void labelClicked();
        };

        class WriteableLine : public QLineEdit
        {
            Q_OBJECT
        friend ExerciseEditorValidator;
        public:
            WriteableLine(QString text, QWidget* parent):QLineEdit(parent){this->setText(text);}
        protected:
            void focusOutEvent(QFocusEvent* ev);
        signals:
            void textMessageBecauseFocusLost(QString t);
        };

        class EditableLine : public QStackedWidget
        {
            Q_OBJECT
        friend ExerciseEditorValidator;
        public:
            EditableLine(QString text,QWidget* parent);
            std::string getContent(){return le->text().toStdString();}
            void setContent(std::string c);
            void setValidationFunction(std::function<bool(std::string)> f){validate_function = f;}
        public slots:
            void showLineEdit(){this->setCurrentWidget(le);}
            void showLabelAndSetText(QString t);            
            void styleTextAsOldAndValid();
            bool validateText();
        signals:
            void textChanged();
        private:
            ClickableLabel* l;
            WriteableLine* le;

            std::function<bool(std::string)> validate_function;
        };
        
        struct MuscleGroups
        {
            MuscleGroups()
            {
                strings.push_back("Shoulder");
                strings.push_back("Back");
                strings.push_back("Chest");
                strings.push_back("Abs");
                strings.push_back("Arms");
                strings.push_back("Legs");
            };

            std::vector<std::string> strings;
        };

        class ExerciseItem : public QWidget
        {
            Q_OBJECT
        friend ExerciseEditorValidator;            
        public:
            ExerciseItem(QWidget* parent);

            std::string getName(){return name->getContent();};
            void setName(std::string n){name->setContent(n);};

            std::string getURL(){return url->getContent();};
            void setURL(std::string u){url->setContent(u);};

            std::string getVideoPath(){return video_path;};
            void setVideoPath(std::string p){video_path = p;};

            std::string getVideoStartTime(){return start_time->getContent();};
            void setVideoStartTime(std::string t){start_time->setContent(t);}

            std::string getVideoEndTime(){return stop_time->getContent();};
            void setVideoEndTime(std::string t){stop_time->setContent(t);}

            std::list<std::string> getMuscleAreas();
            void setMuscleAreas(std::set<FreeFit::Data::MuscleGroup> muscles);
            bool inputIsValid();
            void highlightAsOldAndValid();
            void highlightAsFaulty();
            void setDefaultBackgroundColor(std::string c){default_color = c;}
            void setDefaultBackground();
        private slots:
            void resetStylesheetOnce();
            void itemChanged();
        private:
            void deleteClicked(){emit deleteItemTriggered(this);}
            void downloadClicked(){emit downloadItemTriggered(this);}
            void paintEvent(QPaintEvent* e);
            
            std::string default_color;
            QGridLayout* ly;

            QLabel* name_label;
            QLabel* url_label;
            QLabel* start_time_label;
            QLabel* stop_time_label;

            EditableLine* name;
            EditableLine* url;
            EditableLine* start_time;
            EditableLine* stop_time;

            QPushButton* delete_item;
            QPushButton* download_item;

            MuscleGroups muscle_definitions;
            std::vector<ToggleableLabel*> hashtag_labels;
            bool unchanged = true;
            std::string video_path = "";
        signals:
            void deleteItemTriggered(ExerciseItem*);
            void downloadItemTriggered(ExerciseItem*);
        };

        class ExerciseEditor : public QDialog
        {
            Q_OBJECT
        friend ExerciseEditorValidator;
        public:
            ExerciseEditor(FreeFit::Data::Profile t_p)
                :   p(t_p),demand_handler(),
                    r(t_p.getPathToExerciseDB()),w(t_p.getPathToExerciseDB())
            {                
                ly = new QGridLayout(this);

                button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
                
                connect(button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
                connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

                add_button = new QPushButton("Add Exercise",this);
                download_all_button = new QPushButton("Download All",this);
                connect(add_button,&QPushButton::clicked,this,&ExerciseEditor::addExercise);
                connect(download_all_button,&QPushButton::clicked,this,&ExerciseEditor::downloadAllExercises);

                exercise_area = new QWidget(this);
                exercise_area_ly = new QVBoxLayout(exercise_area);
                exercise_area->setLayout(exercise_area_ly);

                scroll_area = new QScrollArea(this);
                scroll_area->setWidget(exercise_area);
                scroll_area->setWidgetResizable(true);

                r.read();
                for (auto e_data : r.getExerciseList())
                    addExistingExercise(e_data);

                if (exercise_items.empty())
                    addExercise();

                ly->addWidget(add_button,0,0,1,1);
                ly->addWidget(download_all_button,0,3,1,1);
                ly->addWidget(scroll_area,1,0,4,4);
                ly->addWidget(button_box,5,0,4,4,Qt::AlignCenter);
                this->setLayout(ly);
            };
        private:
            QDialogButtonBox* button_box;
            QPushButton* add_button;
            QPushButton* download_all_button;
            QWidget* exercise_area;        
            QVBoxLayout* exercise_area_ly;                
            QScrollArea* scroll_area;
            QGridLayout* ly;
            std::list<ExerciseItem*> exercise_items;

            FreeFit::Data::Profile p;
            FreeFit::Data::BaseXMLReader r;
            FreeFit::Data::ExerciseWriter w;
            FreeFit::Data::DownloadExerciseDemandHandler demand_handler;
        public slots:
            void accept() override
            {   
                std::list<FreeFit::Data::Exercise> lst;
                for (auto e : exercise_items)
                    lst.push_back(exerciseItemToData(e));
                w.createNodeTree(lst);
                w.write();
                QDialog::accept();
            }
        private slots:
            FreeFit::Data::MuscleGroup stringToMuscleGroup(std::string s)
            {
                if (s == "Shoulder")
                    return FreeFit::Data::MuscleGroup::Shoulder;
                else if (s == "Back")
                    return FreeFit::Data::MuscleGroup::Back;
                else if (s == "Chest")
                    return FreeFit::Data::MuscleGroup::Chest;
                else if (s == "Abs")
                    return FreeFit::Data::MuscleGroup::Abs;
                else if (s == "Arms")
                    return FreeFit::Data::MuscleGroup::Arms;
                else if (s == "Legs")
                    return FreeFit::Data::MuscleGroup::Legs;
                else
                    return FreeFit::Data::MuscleGroup::Error;
            }

            FreeFit::Data::Exercise exerciseItemToData(ExerciseItem* e)
            {
                FreeFit::Data::Exercise e_dat;
                e_dat.setName(e->getName());
                e_dat.setVideoURL(e->getURL());
                e_dat.setVideoPath(e->getVideoPath());
                e_dat.setVideoStartTime(e->getVideoStartTime());
                e_dat.setVideoEndTime(e->getVideoEndTime());
                for (auto m : e->getMuscleAreas())
                    e_dat.addTrainedMuscle(stringToMuscleGroup(m));

                return e_dat;
            }

            void registerExerciseItem(ExerciseItem* e)
            {
                exercise_area_ly->addWidget(e);
                connect(e,&ExerciseItem::deleteItemTriggered,this,&ExerciseEditor::deleteExercise);
                connect(e,&ExerciseItem::downloadItemTriggered,this,&ExerciseEditor::downloadExercise);
                exercise_items.push_back(e);
                repaintExerciseBackgrounds();
            }

            void addExercise()
            {
                ExerciseItem* e = new ExerciseItem(this);
                registerExerciseItem(e);
            }

            void addExistingExercise(FreeFit::Data::Exercise e_dat)
            {
                ExerciseItem* e = new ExerciseItem(this);
                e->setName(e_dat.getName());
                e->setVideoPath(e_dat.getVideoPath());
                e->setURL(e_dat.getVideoURL());
                e->setMuscleAreas(e_dat.getTrainedMuscles());
                e->setVideoStartTime(e_dat.getVideoStartTime());
                e->setVideoEndTime(e_dat.getVideoEndTime());

                registerExerciseItem(e);
                e->highlightAsOldAndValid();
            }

            void repaintExerciseBackgrounds()
            {
                unsigned int e_counter = 0;
                for (auto e : exercise_items)
                {
                    if(e_counter % 2 == 0)
                        e->setDefaultBackgroundColor("darkgray");
                    else
                        e->setDefaultBackgroundColor("dimgray");
                    e->setDefaultBackground();
                    ++e_counter;
                }
            }

            std::shared_ptr<DownloadExerciseDemand> generateDownloadExerciseDemand(ExerciseItem* e)
            {
                std::shared_ptr<DownloadExerciseDemand> d = std::make_shared<DownloadExerciseDemand>();
                d->name = e->getName();
                d->video_url = e->getURL();
                d->video_start_time = e->getVideoStartTime();
                d->video_end_time = e->getVideoEndTime();
                d->muscle_areas = e->getMuscleAreas();
                return d;
            }

            void downloadExercise(ExerciseItem* e)
            {
                if(e->inputIsValid())
                {
                    FreeFit::Data::Exercise e_dat = demand_handler.executeDemand(generateDownloadExerciseDemand(e));
                    e->setVideoPath(e_dat.getVideoPath());
                }
                else
                    e->highlightAsFaulty();
            }

            void downloadAllExercises()
            {
                for(auto e : exercise_items)
                    downloadExercise(e);
            }
            
            void deleteExercise(ExerciseItem* e)
            {
                exercise_items.remove(e);
                exercise_area_ly->removeWidget(e);
                disconnect(e,nullptr,nullptr,nullptr);
                delete e;
                repaintExerciseBackgrounds();
            }
        };

        class ExerciseEditorValidator : public QWidget
        {
            Q_OBJECT
        public:
            ExerciseEditorValidator(ExerciseEditor* t_ee):ee(t_ee){};

            void pushAddButton()
            {
                ee->add_button->click();
            }

            int getNumberOfExercises()
            {
                return ee->exercise_items.size();
            }

            std::shared_ptr<DownloadExerciseDemand> getFirstExerciseDemand()
            {
                return ee->generateDownloadExerciseDemand(*(ee->exercise_items.begin()));
            }

            void setFirstExerciseNameText(std::string s)
            {
                ExerciseItem* e = *(ee->exercise_items.begin());
                e->name->le->setText(QString::fromStdString(s));
                e->name->le->textMessageBecauseFocusLost(e->name->le->text());
            }

            void setFirstExerciseURLText(std::string s)
            {
                ExerciseItem* e = *(ee->exercise_items.begin());
                e->url->le->setText(QString::fromStdString(s));
                e->url->le->textMessageBecauseFocusLost(e->url->le->text());
            }

            void setFirstExerciseStartTimeText(std::string s)
            {
                ExerciseItem* e = *(ee->exercise_items.begin());
                e->start_time->le->setText(QString::fromStdString(s));
                e->start_time->le->textMessageBecauseFocusLost(e->start_time->le->text());
            }

            void setFirstExerciseStopTimeText(std::string s)
            {
                ExerciseItem* e = *(ee->exercise_items.begin());
                e->stop_time->le->setText(QString::fromStdString(s));
                e->stop_time->le->textMessageBecauseFocusLost(e->stop_time->le->text());
            }

            void setFirstExerciseMuscleArea(int id)
            {
                ExerciseItem* e = *(ee->exercise_items.begin());
                e->hashtag_labels[id]->clicked();
            }

            void setLastExerciseNameText(std::string s)
            {
                ExerciseItem* e = *(ee->exercise_items.rbegin());
                e->name->le->setText(QString::fromStdString(s));
                e->name->le->textMessageBecauseFocusLost(e->name->le->text());
            }

            void setLastExerciseURLText(std::string s)
            {
                ExerciseItem* e = *(ee->exercise_items.rbegin());
                e->url->le->setText(QString::fromStdString(s));
                e->url->le->textMessageBecauseFocusLost(e->url->le->text());
            }

            void setLastExerciseStartTimeText(std::string s)
            {
                ExerciseItem* e = *(ee->exercise_items.rbegin());
                e->start_time->le->setText(QString::fromStdString(s));
                e->start_time->le->textMessageBecauseFocusLost(e->start_time->le->text());
            }

            void setLastExerciseStopTimeText(std::string s)
            {
                ExerciseItem* e = *(ee->exercise_items.rbegin());
                e->stop_time->le->setText(QString::fromStdString(s));
                e->stop_time->le->textMessageBecauseFocusLost(e->stop_time->le->text());
            }

            void setLastExerciseMuscleArea(int id)
            {
                ExerciseItem* e = *(ee->exercise_items.rbegin());
                e->hashtag_labels[id]->clicked();
            }

            bool isFirstExerciseNameValid()
            {
                ExerciseItem* e = *(ee->exercise_items.begin());
                return e->name->validateText();
            }

            bool isFirstExerciseURLValid()
            {
                ExerciseItem* e = *(ee->exercise_items.begin());
                return e->url->validateText();
            }

            bool isFirstExerciseStartTimeValid()
            {
                ExerciseItem* e = *(ee->exercise_items.begin());
                return e->start_time->validateText();
            }

            bool isFirstExerciseStopTimeValid()
            {
                ExerciseItem* e = *(ee->exercise_items.begin());
                return e->stop_time->validateText();
            }

            void connectToDownloadSignalsOfItems()
            {
                for (auto e : ee->exercise_items)
                    connect(e,&ExerciseItem::downloadItemTriggered,this,&ExerciseEditorValidator::saveDemandFromDownloadClicked);
            }

            void pushFirstDownloadButton()
            {
                ExerciseItem* e = *(ee->exercise_items.begin());
                e->download_item->click();
            }

            void pushLastDownloadButton()
            {
                ExerciseItem* e = *(ee->exercise_items.rbegin());
                e->download_item->click();
            }

            void pushFirstDeleteButton()
            {
                ExerciseItem* e = *(ee->exercise_items.begin());
                e->delete_item->click();
            }

            GUI::DownloadExerciseDemand getLastDemand(){return *last_demand;}
        private:
            ExerciseEditor* ee;
            std::shared_ptr<GUI::DownloadExerciseDemand> last_demand;
        private slots:
            void saveDemandFromDownloadClicked(ExerciseItem* e)
            {
                last_demand = std::shared_ptr<GUI::DownloadExerciseDemand>(ee->generateDownloadExerciseDemand(e));
            }
        };
    }
}

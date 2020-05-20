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

#include "include/newexercisedemand.h"
#include "include/newexercisedemandhandler.h"
#include "include/profile.h"
#include "include/exercise.h"

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
            void mousePressEvent(QMouseEvent* ev) override
            {
                this->clicked();
                QLabel::mousePressEvent(ev);
            }

            void clicked()
            {
                toggled = !toggled;
                if(toggled)
                    this->setStyleSheet("border-color:blue;color:blue;" );
                else
                    this->setStyleSheet("");
            }
        private:
            bool toggled;
        };

        class ClickableLabel : public QLabel
        {
            Q_OBJECT
        friend ExerciseEditorValidator;
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
        friend ExerciseEditorValidator;
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
        friend ExerciseEditorValidator;
        public:
            EditableLine(QString text,QWidget* parent):QStackedWidget(parent)
            {
                this->setFocusPolicy(Qt::StrongFocus);
                l = new ClickableLabel(text,this);
                le = new WriteableLine(text,this);
                this->addWidget(l);
                this->addWidget(le);
                connect(l,&ClickableLabel::labelClicked,this,&EditableLine::showLineEdit);
                connect(le,&WriteableLine::textMessageBecauseFocusLost,this,&EditableLine::showLabelAndSetText);
                connect(le,&QLineEdit::textChanged,this,&EditableLine::validateText);
                connect(le,&QLineEdit::textEdited,this,&EditableLine::textChanged);
            }

            std::string getContent(){return le->text().toStdString();}
            void setContent(std::string c)
            {
                le->setText(QString::fromStdString(c));
                l->setText(QString::fromStdString(c));
            }

            void setValidationFunction(std::function<bool(std::string)> f){validate_function = f;}
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
            
            void styleTextAsOldAndValid()
            {
                l->setStyleSheet("background-color:DarkSeaGreen;");
                le->setStyleSheet("background-color:DarkSeaGreen;");
            }

            bool validateText()
            {
                if(!validate_function(le->text().toStdString()))
                {
                    l->setStyleSheet("background-color:red;");
                    le->setStyleSheet("background-color:red;");
                    return false;
                }
                else
                {
                    l->setStyleSheet("background-color:green;");
                    le->setStyleSheet("background-color:green;");
                    return true;
                }
            }
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
            ExerciseItem(QWidget* parent):QWidget(parent),muscle_definitions(),default_color("black")
            {
                this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
                this->setFocusPolicy(Qt::ClickFocus);

                ly = new QGridLayout(this);
                
                QWidget* hashtag_widget = new QWidget(this);
                QHBoxLayout* hashtag_layout = new QHBoxLayout(hashtag_widget);
                hashtag_widget->setLayout(hashtag_layout);
                for (auto m : muscle_definitions.strings)
                {
                    ToggleableLabel* t = new ToggleableLabel("#" + QString::fromStdString(m),hashtag_widget);
                    hashtag_layout->addWidget(t,0,Qt::AlignLeft);
                    hashtag_labels.push_back(t);
                }

                name_label = new QLabel("Name:",this);
                url_label = new QLabel("Youtube-URL:",this);
                start_time_label = new QLabel("Start Time:",this);
                stop_time_label = new QLabel("Stop Time:",this);

                name_label->setToolTip("No special characters allowed.");
                url_label->setToolTip("Youtube-links only.");
                start_time_label->setToolTip("Enter seconds between 0 and 999.");
                stop_time_label->setToolTip("Enter seconds between 0 and 999.");

                name        = new EditableLine("...",this);
                url         = new EditableLine("...",this);
                start_time  = new EditableLine("...",this);
                stop_time   = new EditableLine("...",this);

                std::regex name_regex("[a-zA-Z0-9\\s]{1,256}");
                auto func_name_regex = [name_regex](std::string s)->bool{return std::regex_match(s,name_regex);};
                name->setValidationFunction(func_name_regex);

                std::regex url_regex("https:\\/\\/www\\.youtube\\.com\\/watch?.*");
                auto func_url_regex = [url_regex](std::string s)->bool{return std::regex_match(s,url_regex);};
                url->setValidationFunction(func_url_regex);

                std::regex int_range_regex("[0-9]{0,3}");
                auto func_int_regex = [int_range_regex](std::string s)->bool{return std::regex_match(s,int_range_regex);};
                start_time->setValidationFunction(func_int_regex);
                stop_time->setValidationFunction(func_int_regex);

                name->validateText();
                url->validateText();
                start_time->validateText();
                stop_time->validateText();
                
                connect(name,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
                connect(url,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
                connect(start_time,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
                connect(stop_time,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);

                delete_item = new QPushButton("Delete",this);
                download_item = new QPushButton("Download",this);
                connect(delete_item,&QPushButton::clicked,this,&ExerciseItem::deleteClicked);
                connect(download_item,&QPushButton::clicked,this,&ExerciseItem::downloadClicked);

                int row_counter = -1;
                int col_counter = -1;

                ly->addWidget(name_label,++row_counter,++col_counter);
                ly->addWidget(url_label,++row_counter,col_counter);
                ly->addWidget(start_time_label,++row_counter,col_counter);
                ly->addWidget(stop_time_label,++row_counter,col_counter);

                row_counter = -1;
                ly->addWidget(name,++row_counter,++col_counter);
                ly->addWidget(url,++row_counter,col_counter);
                ly->addWidget(start_time,++row_counter,col_counter);
                ly->addWidget(stop_time,++row_counter,col_counter);

                ly->addWidget(delete_item,0,++col_counter,row_counter+1,1,Qt::AlignCenter);

                ly->addWidget(download_item,0,++col_counter,row_counter+1,1,Qt::AlignCenter);

                ly->addWidget(hashtag_widget,++row_counter,0,col_counter,2,Qt::AlignLeft);
            }
            
            std::string getName(){return name->getContent();};
            void setName(std::string n){name->setContent(n);};

            std::string getURL(){return url->getContent();};
            void setURL(std::string u){url->setContent(u);};

            std::string getVideoStartTime(){return start_time->getContent();};
            void setVideoStartTime(std::string t){start_time->setContent(t);}

            std::string getVideoEndTime(){return stop_time->getContent();};
            void setVideoEndTime(std::string t){stop_time->setContent(t);}

            std::list<std::string> getMuscleAreas()
            {
                std::list<std::string> l;
                for (auto label : hashtag_labels)
                    if(label->isToggled())
                        l.push_back(label->text().remove(QChar('#')).toStdString());
                return l;
            };

            void setMuscleAreas(std::set<FreeFit::Data::MuscleGroup> muscles)
            {
                for (auto h : hashtag_labels)
                {
                    std::string h_name = h->text().remove(QChar('#')).toStdString();
                    for (auto  m : muscles)
                        if (h_name == muscle_definitions.strings[m])
                            h->click();
                }
            };

            bool inputIsValid()
            {
                return (name->validateText() && url->validateText() && start_time->validateText() && stop_time->validateText());
            }

            void highlightAsOldAndValid()
            {
                this->setStyleSheet("background-color:grey;");
                name->styleTextAsOldAndValid();
                url->styleTextAsOldAndValid();
                start_time->styleTextAsOldAndValid();
                stop_time->styleTextAsOldAndValid();
            }

            void highlightAsFaulty()
            {
                this->setStyleSheet("background-color:red;");
                connect(name,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
                connect(url,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
                connect(start_time,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
                connect(stop_time,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            }

            void setDefaultBackgroundColor(std::string c)
            {
                default_color = c;
            }

            void setDefaultBackground()
            {
                this->setStyleSheet("background-color:" + QString::fromStdString(default_color) + ";");
            }
        private slots:
            void resetStylesheetOnce()
            {
                setDefaultBackground();
                disconnect(name,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
                disconnect(url,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
                disconnect(start_time,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
                disconnect(stop_time,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            }

            void itemChanged()
            {
                unchanged = false;
                disconnect(name,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
                disconnect(url,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
                disconnect(start_time,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
                disconnect(stop_time,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
            }
        private:
            void deleteClicked()
            {
                emit deleteItemTriggered(this);
            }

            void downloadClicked()
            {
                emit downloadItemTriggered(this);
            }

            void paintEvent(QPaintEvent* e)
            {
                QStyleOption opt;
                opt.init(this);
                QPainter p(this);
                style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
            }
            
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
        signals:
            void deleteItemTriggered(ExerciseItem*);
            void downloadItemTriggered(ExerciseItem*);
        };

        class ExerciseEditor : public QDialog
        {
            Q_OBJECT
        friend ExerciseEditorValidator;
        public:
            ExerciseEditor(FreeFit::Data::Profile t_p) : p(t_p),demand_handler(t_p.getPathToExerciseDB())
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
                for (auto e_data : demand_handler.getExerciseList())
                {
                    ExerciseItem* e = addExistingExercise(e_data);
                    e->highlightAsOldAndValid();
                }

                if (exercises_to_download.empty())
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
            std::set<ExerciseItem*> exercises_to_download;

            FreeFit::Data::Profile p;
            FreeFit::Data::NewExerciseDemandHandler demand_handler;
        private slots:
            void registerExerciseItem(ExerciseItem* e)
            {
                exercise_area_ly->addWidget(e);
                connect(e,&ExerciseItem::deleteItemTriggered,this,&ExerciseEditor::deleteExercise);
                connect(e,&ExerciseItem::downloadItemTriggered,this,&ExerciseEditor::downloadExercise);
                exercises_to_download.insert(e);
                repaintExerciseBackgrounds();
            }

            ExerciseItem* addExercise()
            {
                ExerciseItem* e = new ExerciseItem(this);
                registerExerciseItem(e);
                return e;
            }

            ExerciseItem* addExistingExercise(FreeFit::Data::Exercise e_dat)
            {
                ExerciseItem* e = new ExerciseItem(this);
                e->setName(e_dat.getName());
                e->setURL(e_dat.getVideoURL());
                e->setMuscleAreas(e_dat.getTrainedMuscles());
                e->setVideoStartTime(e_dat.getVideoStartTime());
                e->setVideoEndTime(e_dat.getVideoEndTime());
                registerExerciseItem(e);
                return e;
            }

            void repaintExerciseBackgrounds()
            {
                unsigned int e_counter = 0;
                for (auto e : exercises_to_download)
                {
                    if(e_counter % 2 == 0)
                        e->setDefaultBackgroundColor("darkgray");
                    else
                        e->setDefaultBackgroundColor("dimgray");
                    e->setDefaultBackground();
                    ++e_counter;
                }
            }

            NewExerciseDemand* generateNewExerciseDemand(ExerciseItem* e)
            {
                NewExerciseDemand* d = new NewExerciseDemand();
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
                    demand_handler.addDemand(std::shared_ptr<GUI::NewExerciseDemand>(generateNewExerciseDemand(e)));
                    demand_handler.executeDemands();
                }
                else
                    e->highlightAsFaulty();
            }

            void downloadAllExercises()
            {
                for(auto e : exercises_to_download)
                {
                    if(e->inputIsValid())
                        demand_handler.addDemand(std::shared_ptr<GUI::NewExerciseDemand>(generateNewExerciseDemand(e)));
                    else
                        e->highlightAsFaulty();
                }
                demand_handler.executeDemands();
            }
            
            void deleteExercise(ExerciseItem* e)
            {
                exercises_to_download.erase(e);
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
                return ee->exercises_to_download.size();
            }

            NewExerciseDemand* getFirstExerciseDemand()
            {
                return ee->generateNewExerciseDemand(*(ee->exercises_to_download.begin()));
            }

            void setFirstExerciseNameText(std::string s)
            {
                ExerciseItem* e = *(ee->exercises_to_download.begin());
                e->name->le->setText(QString::fromStdString(s));
                e->name->le->textMessageBecauseFocusLost(e->name->le->text());
            }

            void setFirstExerciseURLText(std::string s)
            {
                ExerciseItem* e = *(ee->exercises_to_download.begin());
                e->url->le->setText(QString::fromStdString(s));
                e->url->le->textMessageBecauseFocusLost(e->url->le->text());
            }

            void setFirstExerciseStartTimeText(std::string s)
            {
                ExerciseItem* e = *(ee->exercises_to_download.begin());
                e->start_time->le->setText(QString::fromStdString(s));
                e->start_time->le->textMessageBecauseFocusLost(e->start_time->le->text());
            }

            void setFirstExerciseStopTimeText(std::string s)
            {
                ExerciseItem* e = *(ee->exercises_to_download.begin());
                e->stop_time->le->setText(QString::fromStdString(s));
                e->stop_time->le->textMessageBecauseFocusLost(e->stop_time->le->text());
            }

            void setFirstExerciseMuscleArea(int id)
            {
                ExerciseItem* e = *(ee->exercises_to_download.begin());
                e->hashtag_labels[id]->clicked();
            }

            bool isFirstExerciseNameValid()
            {
                ExerciseItem* e = *(ee->exercises_to_download.begin());
                return e->name->validateText();
            }

            bool isFirstExerciseURLValid()
            {
                ExerciseItem* e = *(ee->exercises_to_download.begin());
                return e->url->validateText();
            }

            bool isFirstExerciseStartTimeValid()
            {
                ExerciseItem* e = *(ee->exercises_to_download.begin());
                return e->start_time->validateText();
            }

            bool isFirstExerciseStopTimeValid()
            {
                ExerciseItem* e = *(ee->exercises_to_download.begin());
                return e->stop_time->validateText();
            }

            void connectToDownloadSignalsOfItems()
            {
                for (auto e : ee->exercises_to_download)
                    connect(e,&ExerciseItem::downloadItemTriggered,this,&ExerciseEditorValidator::saveDemandFromDownloadClicked);
            }

            void pushFirstDownloadButton()
            {
                ExerciseItem* e = *(ee->exercises_to_download.begin());
                e->download_item->click();
            }

            void pushFirstDeleteButton()
            {
                ExerciseItem* e = *(ee->exercises_to_download.begin());
                e->delete_item->click();
            }

            GUI::NewExerciseDemand getLastDemand(){return *last_demand;}
        private:
            ExerciseEditor* ee;
            std::shared_ptr<GUI::NewExerciseDemand> last_demand;
        private slots:
            void saveDemandFromDownloadClicked(ExerciseItem* e)
            {
                last_demand = std::shared_ptr<GUI::NewExerciseDemand>(ee->generateNewExerciseDemand(e));
            }
        };
    }
}

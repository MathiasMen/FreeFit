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
#include <QSize>
#include <QWebEngineView>
#include <QKeyEvent>

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

        class ExerciseEditorBrowser : public QWidget
        {
        Q_OBJECT
        public:
            ExerciseEditorBrowser(QWidget* parent = nullptr) : QWidget(parent)
            {
                this->setMinimumSize(600,600);
                this->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
                ly = new QVBoxLayout(this);

                browser = new QWebEngineView(this);
                address = new QLineEdit(start_url,this);

                connect(address,&QLineEdit::returnPressed,this,&ExerciseEditorBrowser::addressEnterPressed);
                connect(browser,&QWebEngineView::loadStarted,this,&ExerciseEditorBrowser::browserStartedLoading);

                browser->load(start_url);

                ly->addWidget(address);
                ly->addWidget(browser);
                this->setLayout(ly);
            }
        private slots:
            void addressEnterPressed()
            {
                if (validateAddress())
                    browser->load(QUrl(address->text()));
            }

            void browserStartedLoading()
            {
                address->setText(browser->url().toString());
            }

        private:
            bool validateAddress()
            {
                std::regex url_regex("https:\\/\\/www\\.youtube\\.com.*");
                return std::regex_match(address->text().toStdString(),url_regex);
            }

            QVBoxLayout* ly;
            QWebEngineView* browser;
            QLineEdit* address;

            const QString start_url = "https://www.youtube.com";
        };

        class ExerciseEditor : public QDialog
        {
            Q_OBJECT
        friend ExerciseEditorValidator;
        public:
            ExerciseEditor(FreeFit::Data::Profile t_p);
        private:
            QDialogButtonBox* button_box;
            QPushButton* add_button;
            QPushButton* download_all_button;
            QWidget* exercise_area;        
            QVBoxLayout* exercise_area_ly;                
            QScrollArea* scroll_area;
            ExerciseEditorBrowser* browser;
            QGridLayout* ly;
            std::list<ExerciseItem*> exercise_items;

            FreeFit::Data::Profile p;
            FreeFit::Data::ExerciseXMLReader r;
            FreeFit::Data::ExerciseWriter w;
            FreeFit::Data::DownloadExerciseDemandHandler demand_handler;
        public slots:
            void accept() override;
        private slots:
            FreeFit::Data::Exercise exerciseItemToData(ExerciseItem* e);
            void registerExerciseItem(ExerciseItem* e);
            void addExercise();
            void addExistingExercise(FreeFit::Data::Exercise e_dat);
            void repaintExerciseBackgrounds();
            std::shared_ptr<DownloadExerciseDemand> generateDownloadExerciseDemand(ExerciseItem* e);
            void downloadExercise(ExerciseItem* e);
            void downloadAllExercises();            
            void deleteExercise(ExerciseItem* e);
        };

        class ExerciseEditorValidator : public QWidget
        {
            Q_OBJECT
        public:
            ExerciseEditorValidator(ExerciseEditor* t_ee):ee(t_ee){};
            void pushAddButton(){ee->add_button->click();}
            int getNumberOfExercises(){return ee->exercise_items.size();}
            std::shared_ptr<DownloadExerciseDemand> getFirstExerciseDemand();

            void setFirstExerciseNameText(std::string s);
            void setFirstExerciseURLText(std::string s);
            void setFirstExerciseStartTimeText(std::string s);
            void setFirstExerciseStopTimeText(std::string s);
            void setFirstExerciseMuscleArea(int id);
            void setLastExerciseNameText(std::string s);
            void setLastExerciseURLText(std::string s);
            void setLastExerciseStartTimeText(std::string s);
            void setLastExerciseStopTimeText(std::string s);
            void setLastExerciseMuscleArea(int id);
            bool isFirstExerciseNameValid();
            bool isFirstExerciseURLValid();
            bool isFirstExerciseStartTimeValid();
            bool isFirstExerciseStopTimeValid();
            void connectToDownloadSignalsOfItems();
            void pushFirstDownloadButton();
            void pushLastDownloadButton();
            void pushFirstDeleteButton();
            GUI::DownloadExerciseDemand getLastDemand(){return *last_demand;}
        private:
            ExerciseEditor* ee;
            std::shared_ptr<GUI::DownloadExerciseDemand> last_demand;
        private slots:
            void saveDemandFromDownloadClicked(ExerciseItem* e);
        };
    }
}

#pragma once

#include <QApplication>
#include <QString>
#include <QLabel>
#include <QDialog>
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
#include <QSize>
#include <QWebEngineView>
#include <QKeyEvent>
#include <QMovie>
#include <QSpacerItem>
#include <QMessageBox>
#include <QProgressDialog>

#include <set>
#include <functional>
#include <regex>
#include <memory>
#include <math.h>
#include <future>
#include <thread>
#include <utility>

#include "include/downloadexercisedemand.h"
#include "include/downloadexercisedemandhandler.h"
#include "include/profile.h"
#include "include/exercise.h"
#include "include/datadefinitions.h"
#include "include/xmlreader.h"
#include "include/controls.h"
#include "include/materialbutton.h"
#include "include/materialtextfield.h"
#include "include/materialdialog.h"
#include "include/materialslider.h"

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
            ToggleableLabel(QString text, QWidget* parent);
            void setSelectable(bool s);
            bool isToggled(){return toggled;}
            void click(){clicked_impl();};
        signals:
            void clicked();
        protected:
            void mousePressEvent(QMouseEvent* ev) override;
        private slots:
            void clicked_impl();
        private:
            bool toggled = false;
            QString css_string;
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

            std::string getName(){return name->text().toStdString();};
            void setName(std::string n){name->setText(QString::fromStdString(n)); name->validateText();};

            std::string getURL(){return url->text().toStdString();};
            void setURL(std::string u){url->setText(QString::fromStdString(u)); url->validateText();};

            std::string getVideoPath(){return video_path;};
            void setVideoPath(std::string p){video_path = p;};

            std::string getThumbnailPath(){return thumbnail_path;}
            void setThumbnailPath(std::string t){thumbnail_path = t;}

            std::list<std::string> getMuscleAreas();
            void setMuscleAreas(std::set<FreeFit::Data::MuscleGroup> muscles);
            bool inputIsValid();
            void highlightAsFaulty();
            void setDefaultBackgroundColor(std::string c){default_color = c;}
            void setDefaultBackground();
            void showWaitingSymbol();
            void hideWaitingSymbol();
        public slots:
            std::string getVideoStartTime(){return start_time_lbl->text().toStdString();};
            void setVideoStartTime(std::string t);
            void setVideoStartTime(ExerciseItem* e, std::string t);

            std::string getVideoEndTime(){return stop_time_lbl->text().toStdString();};
            void setVideoEndTime(std::string t);
            void setVideoEndTime(ExerciseItem* e, std::string t);
        signals:
            void urlChange(ExerciseItem*,std::string);
        private slots:
            void resetStylesheetOnce();
            void itemChanged();
            void urlChanged();
            void sliderChanged(int,int);
            void setSliderRange(int,int);
            void setSliderRange(ExerciseItem*,int,int);
        private:
            void deleteClicked(){emit deleteItemTriggered(this);}
            void downloadClicked(){emit downloadItemTriggered(this);}
            void paintEvent(QPaintEvent* e);
            std::string timeIntToString(int);
            std::string default_color;
            QGridLayout* ly;

            QMovie *processMovie;
            QLabel *processLabel;

            MaterialTextField* name;
            MaterialTextField* url;
            MaterialSlider* start_stop;

            QLabel* start_time_lbl;
            QLabel* stop_time_lbl;

            QLabel* item_downloaded_icon;
            QLabel* item_downloaded_text;
            MaterialButton* delete_item;

            MuscleGroups muscle_definitions;
            std::vector<ToggleableLabel*> hashtag_labels;
            bool unchanged = true;
            std::string video_path = "";
            std::string thumbnail_path = "";
        signals:
            void deleteItemTriggered(ExerciseItem*);
            void downloadItemTriggered(ExerciseItem*);
        };

        class ExerciseEditorBrowser : public QWidget
        {
        Q_OBJECT
        public:
            ExerciseEditorBrowser(QWidget* parent = nullptr);
            QWebEngineView* getWebEngineView(){return browser;};
        private slots:
            void addressEnterPressed();
            void browserStartedLoading();
        private:
            bool validateAddress();

            QVBoxLayout* ly;
            QWebEngineView* browser;
            MaterialTextField* address;

            const QString start_url = "https://www.youtube.com";
        };

        class ExerciseEditor : public MaterialDialog
        {
            Q_OBJECT
        friend ExerciseEditorValidator;
        public:
            ExerciseEditor(FreeFit::Data::Profile t_p);
            void deleteProfile(){delete browser->getWebEngineView()->page();};
            std::list<FreeFit::Data::Exercise> getExerciseData();
        private:
            MaterialButton* add_button;
            MaterialButton* download_all_button;
            QLabel* new_exercise_label;
            QWidget* new_exercise_area;        
            QVBoxLayout* new_exercise_area_ly;                
            QScrollArea* new_exercise_scroll_area;
            QPushButton* download_exercises_button;
            QLabel* old_exercise_label;
            QWidget* old_exercise_area;
            QVBoxLayout* old_exercise_area_ly;
            QScrollArea* old_exercise_scroll_area;
            ExerciseEditorBrowser* browser;
            ControlButton* next_page_button;
            ControlButton* previous_page_button;
            QGridLayout* ly;

            std::list<ExerciseItem*> exercise_items;
            std::list<ExerciseItem*> new_exercise_items;

            FreeFit::Data::Profile p;
            FreeFit::Data::ExerciseXMLReader r;
            FreeFit::Data::ExerciseWriter w;
            FreeFit::Data::DownloadExerciseDemandHandler demand_handler;
            FreeFit::Data::InfoExerciseDemandHandler info_demand_handler;
        signals:
            void exerciseDownloaded(ExerciseItem* e);
            void setExerciseStartTimeSignal(ExerciseItem*,std::string);
            void setExerciseEndTimeSignal(ExerciseItem*,std::string);
            void setExerciseSliderRange(ExerciseItem*,int,int);
        public slots:
            void accept() override;
            void reject() override;
            void exerciseUrlChanged(ExerciseItem*,std::string);
        private slots:
            FreeFit::Data::Exercise exerciseItemToData(ExerciseItem* e);
            int timeFormatStringToSecondsInt(std::string);
            std::string secondsIntToTimeFormatString(int);
            void registerExerciseItem(ExerciseItem* e);
            void addExercise();
            void addExistingExercise(FreeFit::Data::Exercise e_dat);
            void moveExerciseToExisting(ExerciseItem* e);
            std::shared_ptr<DownloadExerciseDemand> generateDownloadExerciseDemand(ExerciseItem* e);
            bool downloadExercise(ExerciseItem* e);
            void downloadAllExercises();            
            void deleteExercise(ExerciseItem* e);
            void writeXML();
        };

        class ExerciseEditorValidator : public QWidget
        {
            Q_OBJECT
        public:
            ExerciseEditorValidator(ExerciseEditor* t_ee):ee(t_ee){};
            void pushAddButton(){ee->add_button->click();}
            int getNumberOfExercises(){return ee->exercise_items.size();}
            int getNumberOfExercisesToDownload(){return ee->new_exercise_items.size();}
            std::shared_ptr<DownloadExerciseDemand> getFirstExerciseDemand();

            void setOutPath(std::string s);

            void setFirstNewExerciseNameText(std::string s);
            void setFirstNewExerciseURLText(std::string s);
            void setFirstNewExerciseMuscleArea(int id);
            void setLastNewExerciseNameText(std::string s);
            void setLastNewExerciseURLText(std::string s);
            void setLastNewExerciseMuscleArea(int id);
            std::string getFirstNewExerciseNameText();
            std::string getFirstNewExerciseURLText();
            std::string getFirstNewExerciseStartTime();
            std::string getFirstNewExerciseStopTime();
            std::string getLastNewExerciseNameText();
            std::string getLastNewExerciseURLText();
            std::string getLastNewExerciseStartTime();
            std::string getLastNewExerciseStopTime();
            bool isFirstNewExerciseMuscleAreaSelected(int id);
            bool isLastNewExerciseMuscleAreaSelected(int id);
            bool isFirstNewExerciseNameValid();
            bool isFirstNewExerciseURLValid();

            void setFirstOldExerciseNameText(std::string s);
            void setFirstOldExerciseURLText(std::string s);
            void setFirstOldExerciseMuscleArea(int id);
            void setLastOldExerciseNameText(std::string s);
            void setLastOldExerciseURLText(std::string s);
            void setLastOldExerciseMuscleArea(int id);
            std::string getFirstOldExerciseNameText();
            std::string getFirstOldExerciseURLText();
            std::string getFirstOldExerciseStartTime();
            std::string getFirstOldExerciseStopTime();
            std::string getLastOldExerciseNameText();
            std::string getLastOldExerciseURLText();
            std::string getLastOldExerciseStartTime();
            std::string getLastOldExerciseStopTime();
            bool isFirstOldExerciseMuscleAreaSelected(int id);
            bool isLastOldExerciseMuscleAreaSelected(int id);
            bool isFirstOldExerciseNameValid();
            bool isFirstOldExerciseURLValid();
            bool isLastOldExerciseNameValid();
            bool isLastOldExerciseURLValid();

            void connectToDownloadSignalsOfItems();
            void pushDownloadAllButton();
            void pushFirstDeleteButtonOldExercises();
            void pushFirstDeleteButtonNewExercises();
            GUI::DownloadExerciseDemand getLastDemand(){return *last_demand;}
        private:
            ExerciseEditor* ee;
            std::shared_ptr<GUI::DownloadExerciseDemand> last_demand;
        private slots:
            void saveDemandFromDownloadClicked(ExerciseItem* e);
        };
    }
}

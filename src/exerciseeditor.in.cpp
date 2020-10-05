#include "include/exerciseeditor.h"

namespace FreeFit
{
    namespace GUI
    {
        ExerciseItem::ExerciseItem(QWidget* parent):QWidget(parent),muscle_definitions(),default_color("black")
        {
            this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            this->setFocusPolicy(Qt::ClickFocus);

            ly = new QGridLayout(this);
            
            hashtag_widget = new HashtagBar(this);
            for (auto m : muscle_definitions.strings)
                hashtag_widget->addHashtag(m);

            processMovie = new QMovie("${CMAKE_SOURCE_DIR}/tools/loading.gif");
            processLabel = new QLabel(this);
            processLabel->setMovie(processMovie);
            processLabel->hide();

            name        = new MaterialTextField("Name",this);
            url         = new MaterialTextField("URL",this);
            start_stop  = new MaterialSlider(this,10,20);
            start_time_lbl = new QLabel("00:00",this);
            stop_time_lbl = new QLabel("00:00",this);

            name->setToolTip("No special characters allowed.");
            url->setToolTip("Youtube-links only.");

            std::regex name_regex("[a-zA-Z0-9\\s]{1,256}");
            auto func_name_regex = [name_regex](std::string s)->bool{return std::regex_match(s,name_regex);};
            name->setValidationFunction(func_name_regex);

            std::regex url_regex("https:\\/\\/www\\.youtube\\.com\\/watch?.*");
            auto func_url_regex = [url_regex](std::string s)->bool{return std::regex_match(s,url_regex);};
            url->setValidationFunction(func_url_regex);

            name->validateText();
            url->validateText();
            
            connect(name,&QLineEdit::textChanged,this,&ExerciseItem::itemChanged);
            connect(url,&QLineEdit::textChanged,this,&ExerciseItem::itemChanged);
            
            connect(start_stop,&MaterialSlider::valuesChanged,this,&ExerciseItem::sliderChanged);
            connect(url,&QLineEdit::textChanged,this,&ExerciseItem::urlChanged);

            item_downloaded_icon = new QLabel("",this);
            item_downloaded_text = new QLabel("",this);
            delete_item = new MaterialButton("Delete",this);
            connect(delete_item,&QPushButton::clicked,this,&ExerciseItem::deleteClicked);

            int row_counter = -1;
            int col_counter = -1;

            row_counter = -1;
            ly->addWidget(name,++row_counter,++col_counter);
            ly->addWidget(url,++row_counter,col_counter);

            QHBoxLayout* slider_ly = new QHBoxLayout;
            slider_ly->addWidget(start_time_lbl,Qt::AlignRight);
            slider_ly->addWidget(start_stop,Qt::AlignHCenter);
            slider_ly->addWidget(stop_time_lbl,Qt::AlignLeft);

            ly->addLayout(slider_ly,++row_counter,col_counter);

            ly->addWidget(delete_item,0,++col_counter,row_counter+1,1,Qt::AlignCenter);
            ly->addWidget(processLabel,0,++col_counter,row_counter+1,1,Qt::AlignCenter);
            ly->addWidget(item_downloaded_text,0,++col_counter,row_counter+1,1,Qt::AlignCenter);
            ly->addWidget(item_downloaded_icon,0,++col_counter,row_counter+1,1,Qt::AlignCenter);

            ly->addWidget(hashtag_widget,++row_counter,0,col_counter,2,Qt::AlignLeft);
        }

        void ExerciseItem::setColor(std::string c)
        {
            start_time_lbl->setStyleSheet(QString::fromStdString("color:" + c + ";"));
            stop_time_lbl->setStyleSheet(QString::fromStdString("color:" + c + ";"));
            delete_item->setColor(c);
            start_stop->setColor(c);
            hashtag_widget->setColor(c);
        }

        std::list<std::string> ExerciseItem::getMuscleAreas()
        {
            return hashtag_widget->getToggledStrings();
        }

        void ExerciseItem::setMuscleAreas(std::set<FreeFit::Data::MuscleGroup> muscles)
        {
            for (auto m : muscles)
                hashtag_widget->toggleHashtag(muscle_definitions.strings[m]);
        }

        bool ExerciseItem::inputIsValid()
        {
            return (name->validateText() && url->validateText());
        }

        void ExerciseItem::highlightAsFaulty()
        {
            this->setStyleSheet("background-color:red;");
            connect(name,&QLineEdit::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            connect(url,&QLineEdit::textChanged,this,&ExerciseItem::resetStylesheetOnce);
        }

        void ExerciseItem::setDefaultBackground()
        {
            this->setStyleSheet("background-color:" + QString::fromStdString(default_color) + ";");
        }

        void ExerciseItem::showWaitingSymbol()
        {
            processMovie->start();
            processLabel->show();
        }

        void ExerciseItem::hideWaitingSymbol()
        {
            processMovie->stop();
            processLabel->hide();
        }

        void ExerciseItem::setVideoStartTime(std::string t)
        {
            start_time_lbl->setText(QString::fromStdString(t));
        }

        void ExerciseItem::setVideoStartTime(ExerciseItem* e, std::string t)
        {
            if (e == this)
                start_time_lbl->setText(QString::fromStdString(t));
        }

        void ExerciseItem::setVideoEndTime(std::string t)
        {
            stop_time_lbl->setText(QString::fromStdString(t));
        }

        void ExerciseItem::setVideoEndTime(ExerciseItem* e, std::string t)
        {
            if (e == this)
                stop_time_lbl->setText(QString::fromStdString(t));
        }

        void ExerciseItem::resetStylesheetOnce()
        {
            setDefaultBackground();
            disconnect(name,&QLineEdit::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            disconnect(url,&QLineEdit::textChanged,this,&ExerciseItem::resetStylesheetOnce);
        }

        void ExerciseItem::itemChanged()
        {
            unchanged = false;
            disconnect(name,&QLineEdit::textChanged,this,&ExerciseItem::itemChanged);
            disconnect(url,&QLineEdit::textChanged,this,&ExerciseItem::itemChanged);
        }

        void ExerciseItem::urlChanged()
        {
            if(url->validateText())
                emit urlChange(this,url->text().toStdString());
        }

        void ExerciseItem::sliderChanged(int start, int stop)
        {
            start_time_lbl->setText(QString::fromStdString(timeIntToString(start)));
            stop_time_lbl->setText(QString::fromStdString(timeIntToString(stop)));
        }

        void ExerciseItem::setSliderRange(int start, int stop)
        {
            start_stop->setMinValue(start);
            start_stop->setMaxValue(stop);
        }

        void ExerciseItem::setSliderRange(ExerciseItem* e, int start, int stop)
        {
            if (e == this)
            {
                start_stop->setMinValue(start);
                start_stop->setMaxValue(stop);
            }
        }

        std::string ExerciseItem::timeIntToString(int i)
        {
            int mins = (int)floor(i/60);
            int secs = i - mins*60;
            std::string s_m = (mins > 9 ? std::to_string(mins) : std::string("0" + std::to_string(mins)));
            std::string s_s = (secs > 9 ? std::to_string(secs) : std::string("0" + std::to_string(secs)));
            return s_m + ":" + s_s;
        }

        void ExerciseItem::paintEvent(QPaintEvent* e)
        {
            QStyleOption opt;
            opt.init(this);
            QPainter p(this);
            style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
        }

        ExistingExerciseViewer::ExistingExerciseViewer(QWidget* parent) : QWidget(parent)
        {
        }

        ExerciseEditorBrowser::ExerciseEditorBrowser(QWidget* parent) : QWidget(parent)
        {
            this->setMinimumSize(600,600);
            this->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
            ly = new QVBoxLayout(this);

            browser = new QWebEngineView(this);
            address = new MaterialTextField("Youtube-URL",this);

            connect(address,&QLineEdit::returnPressed,this,&ExerciseEditorBrowser::addressEnterPressed);
            connect(browser,&QWebEngineView::loadStarted,this,&ExerciseEditorBrowser::browserStartedLoading);

            browser->load(start_url);

            ly->addWidget(address);
            ly->addWidget(browser);
            this->setLayout(ly);
        }
        
        void ExerciseEditorBrowser::addressEnterPressed()
        {
            if (validateAddress())
                browser->load(QUrl(address->text()));
        }

        void ExerciseEditorBrowser::browserStartedLoading()
        {
            address->setText(browser->url().toString());
        }

        bool ExerciseEditorBrowser::validateAddress()
        {
            std::regex url_regex("https:\\/\\/www\\.youtube\\.com.*");
            return std::regex_match(address->text().toStdString(),url_regex);
        }

        ExerciseEditor::ExerciseEditor(FreeFit::Data::Profile t_p)
            :   MaterialDialog("Workout Type","Profile",""),p(t_p),demand_handler(),info_demand_handler(),
                r(t_p.getPathToExerciseDB()),w(t_p.getPathToExerciseDB())
        {
            connect(getAcceptButton(), &QPushButton::clicked, this, &QDialog::accept);
            connect(getRejectButton(), &QPushButton::clicked, this, &QDialog::reject);

            new_exercises_widget = new QWidget;
            QHBoxLayout* new_exercises_widget_ly = new QHBoxLayout(new_exercises_widget);
            browser = new ExerciseEditorBrowser(this);
            QGridLayout* new_exercises_right_panel_ly = new QGridLayout;
            new_exercise_label = new QLabel("New Exercises",this);
            new_exercise_area = new QWidget(this);
            new_exercise_area_ly = new QVBoxLayout(new_exercise_area);
            new_exercise_area_ly->addStretch();
            new_exercise_area->setLayout(new_exercise_area_ly);
            new_exercise_scroll_area = new QScrollArea(this);
            new_exercise_scroll_area->setWidget(new_exercise_area);
            new_exercise_scroll_area->setAlignment(Qt::AlignTop);
            new_exercise_scroll_area->setWidgetResizable(true);
            download_exercises_button = new MaterialButton("Add to existing exercises",this);
            connect(download_exercises_button,&QPushButton::clicked,this,&ExerciseEditor::downloadAllExercises);
            add_button = new MaterialButton("Create new exercise",this);
            connect(add_button,&QPushButton::clicked,this,&ExerciseEditor::addExercise);
            new_exercises_right_panel_ly->addWidget(add_button,0,1,1,2);
            new_exercises_right_panel_ly->addWidget(new_exercise_label,1,1,1,2);
            new_exercises_right_panel_ly->addWidget(new_exercise_scroll_area,2,1,1,2);
            new_exercises_right_panel_ly->addWidget(download_exercises_button,3,1,1,2);
            new_exercises_widget_ly->addWidget(browser);
            new_exercises_widget_ly->addLayout(new_exercises_right_panel_ly);

            old_exercises_widget = new QWidget;
            QVBoxLayout* old_exercises_widget_ly = new QVBoxLayout(old_exercises_widget);
            old_exercises_viewer = new ExistingExerciseViewer(this);
            old_exercise_area_ly = new QVBoxLayout(old_exercises_viewer);
            old_exercise_area_ly->addStretch();
            old_exercises_viewer->setLayout(old_exercise_area_ly);
            old_exercise_scroll_area = new QScrollArea(this);
            old_exercise_scroll_area->setWidget(old_exercises_viewer);
            old_exercise_scroll_area->setAlignment(Qt::AlignTop);
            old_exercise_scroll_area->setWidgetResizable(true);
            old_exercise_label = new QLabel("Existing Exercises",this);
            old_exercises_widget_ly->addWidget(old_exercise_label);
            old_exercises_widget_ly->addWidget(old_exercise_scroll_area);

            tab_widget = new QTabWidget(this);
            tab_widget->setTabPosition(QTabWidget::North);
            tab_widget->addTab(new_exercises_widget,"Add new Exercises");
            tab_widget->addTab(old_exercises_widget,"Edit old Exercises");            
            tab_widget->setStyleSheet("QTabBar::tab { height: 40px; width: 100px; }");
            addWidget(tab_widget,0,0);

            r.read();
            for (auto e_data : r.getExerciseList())
                addExistingExercise(e_data);

            if (exercise_items.empty())
                addExercise();
        }
        
        void ExerciseEditor::setColor(std::string c)
        {
            MaterialDialog::setColor(c);
            new_exercise_label->setStyleSheet(QString::fromStdString("color:" + c + ";"));
            add_button->setColor(c);
            download_exercises_button->setColor(c);
            old_exercise_label->setStyleSheet(QString::fromStdString("color:" + c + ";"));

            for (auto ex : exercise_items)
                ex->setColor(c);
            for (auto new_ex : new_exercise_items)
                new_ex->setColor(c);
        }

        void ExerciseEditor::accept()
        {   
            writeXML();
            QDialog::accept();
        }

        void ExerciseEditor::reject()
        {
            if (!new_exercise_items.empty())
            {
                QMessageBox reject_box;
                reject_box.setText("Some exercises have not been downloaded yet. Properties of these will be lost. Continue?");
                reject_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                int res = reject_box.exec();
                if (res == QMessageBox::No)
                    return;
            }
            writeXML();
            QDialog::reject();
        }

        void ExerciseEditor::writeXML()
        {
            std::list<FreeFit::Data::Exercise> lst;
            for (auto e : exercise_items)
                lst.push_back(exerciseItemToData(e));
            w.createNodeTree(lst);
            w.write();
        }

        FreeFit::Data::Exercise ExerciseEditor::exerciseItemToData(ExerciseItem* e)
        {
            FreeFit::Data::Exercise e_dat;
            e_dat.setName(e->getName());
            e_dat.setVideoURL(e->getURL());
            e_dat.setVideoPath(e->getVideoPath());
            e_dat.setThumbnailPath(e->getThumbnailPath());
            e_dat.setVideoStartTime(std::to_string(timeFormatStringToSecondsInt(e->getVideoStartTime())));
            e_dat.setVideoEndTime(std::to_string(timeFormatStringToSecondsInt(e->getVideoEndTime())));
            for (auto m : e->getMuscleAreas())
                e_dat.addTrainedMuscle(FreeFit::Data::stringToMuscleGroup(m));

            return e_dat;
        }

        int ExerciseEditor::timeFormatStringToSecondsInt(std::string s)
        {   
            size_t colon_pos = s.find(":");
            std::string mins, secs;
            if (colon_pos == 1)
            {
                mins += "0";
                mins += s[0];
            }
            else if (colon_pos == 2)
                mins = s.substr(0,2);
            else
                mins = "00";
            secs = s.substr(colon_pos+1,colon_pos+2);
            return std::stoi(mins)*60 + std::stoi(secs);
        }

        std::string ExerciseEditor::secondsIntToTimeFormatString(int i)
        {
            int mins = (int)floor(i/60);
            int secs = i - mins*60;
            std::string s_m = (mins > 9 ? std::to_string(mins) : std::string("0" + std::to_string(mins)));
            std::string s_s = (secs > 9 ? std::to_string(secs) : std::string("0" + std::to_string(secs)));
            return s_m + ":" + s_s;
        }

        void ExerciseEditor::registerExerciseItem(ExerciseItem* e)
        {
            e->inputIsValid();
            old_exercise_area_ly->insertWidget(0,e);
            connect(e,&ExerciseItem::deleteItemTriggered,this,&ExerciseEditor::deleteExercise);
            connect(e,&ExerciseItem::downloadItemTriggered,this,&ExerciseEditor::downloadExercise);
            exercise_items.push_back(e);
        }

        void ExerciseEditor::addExercise()
        {
            ExerciseItem* e = new ExerciseItem(this);
            e->setColor(color);
            new_exercise_items.push_back(e);
            new_exercise_area_ly->insertWidget(0,e);
            connect(e,&ExerciseItem::deleteItemTriggered,this,&ExerciseEditor::deleteExercise);
            connect(e,&ExerciseItem::downloadItemTriggered,this,&ExerciseEditor::downloadExercise);
            connect(this,SIGNAL(exerciseDownloaded(ExerciseItem*)),this,SLOT(moveExerciseToExisting(ExerciseItem*)));
            connect(e,SIGNAL(urlChange(ExerciseItem*,std::string)),this,SLOT(exerciseUrlChanged(ExerciseItem*,std::string)));
            connect(this,SIGNAL(setExerciseStartTimeSignal(ExerciseItem*,std::string)),e,SLOT(setVideoStartTime(ExerciseItem*,std::string)));
            connect(this,SIGNAL(setExerciseEndTimeSignal(ExerciseItem*,std::string)),e,SLOT(setVideoEndTime(ExerciseItem*,std::string)));
            connect(this,SIGNAL(setExerciseSliderRange(ExerciseItem*,int,int)),e,SLOT(setSliderRange(ExerciseItem*,int,int)));
        }

        void ExerciseEditor::addExistingExercise(FreeFit::Data::Exercise e_dat)
        {
            ExerciseItem* e = new ExerciseItem(this);
            e->setName(e_dat.getName());
            e->setVideoPath(e_dat.getVideoPath());
            e->setThumbnailPath(e_dat.getThumbnailPath());
            e->setURL(e_dat.getVideoURL());
            e->setMuscleAreas(e_dat.getTrainedMuscles());
            e->setVideoStartTime(secondsIntToTimeFormatString(std::stoi(e_dat.getVideoStartTime())));
            e->setVideoEndTime(secondsIntToTimeFormatString(std::stoi(e_dat.getVideoEndTime())));

            registerExerciseItem(e);
        }

        void ExerciseEditor::exerciseUrlChanged(ExerciseItem* e,std::string url)
        {
            std::shared_ptr<InfoExerciseDemand> d = std::make_shared<InfoExerciseDemand>();
            d->video_url = url;
            int video_length = info_demand_handler.executeDemand(d);
            emit setExerciseEndTimeSignal(e,secondsIntToTimeFormatString(video_length));
            emit setExerciseStartTimeSignal(e,secondsIntToTimeFormatString(0));
            emit setExerciseSliderRange(e,0,video_length);
        }

        std::shared_ptr<DownloadExerciseDemand> ExerciseEditor::generateDownloadExerciseDemand(ExerciseItem* e)
        {
            std::shared_ptr<DownloadExerciseDemand> d = std::make_shared<DownloadExerciseDemand>();
            d->name = e->getName();
            d->video_url = e->getURL();
            d->video_start_time = std::to_string(timeFormatStringToSecondsInt(e->getVideoStartTime()));
            d->video_end_time = std::to_string(timeFormatStringToSecondsInt(e->getVideoEndTime()));
            d->muscle_areas = e->getMuscleAreas();
            return d;
        }

        void ExerciseEditor::moveExerciseToExisting(ExerciseItem* e)
        {
            e->hideWaitingSymbol();
            registerExerciseItem(e);
            new_exercise_items.remove(e);
        }

        bool ExerciseEditor::downloadExercise(ExerciseItem* e)
        {
            if(e->inputIsValid())
            {
                QProgressDialog progress("Downloading files...", QString(), 0, 1, this);
                progress.setWindowModality(Qt::WindowModal);     
                progress.show();

                FreeFit::Data::Exercise e_dat = demand_handler.executeDemand(generateDownloadExerciseDemand(e));
                e->setVideoPath(e_dat.getVideoPath());
                e->setThumbnailPath(e_dat.getThumbnailPath());
                emit exerciseDownloaded(e);
                return true;
            }
            else
            {
                e->highlightAsFaulty();
                return false;
            }
        }
        
        void ExerciseEditor::downloadAllExercises()
        {
            std::list<ExerciseItem*> tmp_exercise_list = new_exercise_items;
            for(auto e : tmp_exercise_list)
                downloadExercise(e);
        }

        void ExerciseEditor::deleteExercise(ExerciseItem* e)
        {
            exercise_items.remove(e);
            new_exercise_items.remove(e);
            new_exercise_area_ly->removeWidget(e);
            disconnect(e,nullptr,nullptr,nullptr);
            delete e;
        }

        std::list<FreeFit::Data::Exercise> ExerciseEditor::getExerciseData()
        {
            return r.getExerciseList();
        }

        std::shared_ptr<DownloadExerciseDemand> ExerciseEditorValidator::getFirstExerciseDemand()
        {
            return ee->generateDownloadExerciseDemand(*(ee->new_exercise_items.begin()));
        }

        void ExerciseEditorValidator::setOutPath(std::string s)
        {
            ee->w = FreeFit::Data::ExerciseWriter(s);
        }

        void ExerciseEditorValidator::setFirstNewExerciseNameText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            e->name->setText(QString::fromStdString(s));
        }

        void ExerciseEditorValidator::setFirstNewExerciseURLText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            e->url->setText(QString::fromStdString(s));
        }

        void ExerciseEditorValidator::setFirstNewExerciseMuscleArea(int id)
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            e->hashtag_widget->toggleHashtag(e->muscle_definitions.strings[id]);
        }

        void ExerciseEditorValidator::setLastNewExerciseNameText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            e->name->setText(QString::fromStdString(s));
        }

        void ExerciseEditorValidator::setLastNewExerciseURLText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            e->url->setText(QString::fromStdString(s));
        }

        void ExerciseEditorValidator::setLastNewExerciseMuscleArea(int id)
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            e->hashtag_widget->toggleHashtag(e->muscle_definitions.strings[id]);
        }

        std::string ExerciseEditorValidator::getFirstNewExerciseNameText()
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            return e->name->text().toStdString();
        }

        std::string ExerciseEditorValidator::getFirstNewExerciseURLText()
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            return e->url->text().toStdString();
        }

        std::string ExerciseEditorValidator::getFirstNewExerciseStartTime()
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            return e->getVideoStartTime();
        }

        std::string ExerciseEditorValidator::getFirstNewExerciseStopTime()
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            return e->getVideoEndTime();            
        }

        std::string ExerciseEditorValidator::getLastNewExerciseNameText()
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            return e->name->text().toStdString();
        }

        std::string ExerciseEditorValidator::getLastNewExerciseURLText()
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            return e->url->text().toStdString();
        }

        std::string ExerciseEditorValidator::getLastNewExerciseStartTime()
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            return e->getVideoStartTime();
        }

        std::string ExerciseEditorValidator::getLastNewExerciseStopTime()
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            return e->getVideoEndTime();            
        }

        bool ExerciseEditorValidator::isFirstNewExerciseMuscleAreaSelected(int id)
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            return e->hashtag_widget->isHashtagToggled(e->muscle_definitions.strings[id]);
        }

        bool ExerciseEditorValidator::isLastNewExerciseMuscleAreaSelected(int id)
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            return e->hashtag_widget->isHashtagToggled(e->muscle_definitions.strings[id]);
        }

        bool ExerciseEditorValidator::isFirstNewExerciseNameValid()
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            return e->name->validateText();
        }

        bool ExerciseEditorValidator::isFirstNewExerciseURLValid()
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            return e->url->validateText();
        }

        void ExerciseEditorValidator::setFirstOldExerciseNameText(std::string s)
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            e->name->setText(QString::fromStdString(s));
        }
        
        void ExerciseEditorValidator::setFirstOldExerciseURLText(std::string s)
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            e->url->setText(QString::fromStdString(s));
        }

        void ExerciseEditorValidator::setFirstOldExerciseMuscleArea(int id)
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            e->hashtag_widget->toggleHashtag(e->muscle_definitions.strings[id]);
        }

        void ExerciseEditorValidator::setLastOldExerciseNameText(std::string s)
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            e->name->setText(QString::fromStdString(s));
        }

        void ExerciseEditorValidator::setLastOldExerciseURLText(std::string s)
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            e->url->setText(QString::fromStdString(s));
        }

        void ExerciseEditorValidator::setLastOldExerciseMuscleArea(int id)
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            e->hashtag_widget->toggleHashtag(e->muscle_definitions.strings[id]);
        }

        std::string ExerciseEditorValidator::getFirstOldExerciseNameText()
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            return e->name->text().toStdString();
        }

        std::string ExerciseEditorValidator::getFirstOldExerciseURLText()
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            return e->url->text().toStdString();
        }

        std::string ExerciseEditorValidator::getFirstOldExerciseStartTime()
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            return e->getVideoStartTime();
        }

        std::string ExerciseEditorValidator::getFirstOldExerciseStopTime()
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            return e->getVideoEndTime();            
        }

        std::string ExerciseEditorValidator::getLastOldExerciseNameText()
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            return e->name->text().toStdString();
        }

        std::string ExerciseEditorValidator::getLastOldExerciseURLText()
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            return e->url->text().toStdString();
        }

        std::string ExerciseEditorValidator::getLastOldExerciseStartTime()
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            return e->getVideoStartTime();
        }

        std::string ExerciseEditorValidator::getLastOldExerciseStopTime()
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            return e->getVideoEndTime();            
        }

        bool ExerciseEditorValidator::isFirstOldExerciseMuscleAreaSelected(int id)
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            return e->hashtag_widget->isHashtagToggled(e->muscle_definitions.strings[id]);
        }

        bool ExerciseEditorValidator::isLastOldExerciseMuscleAreaSelected(int id)
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            return e->hashtag_widget->isHashtagToggled(e->muscle_definitions.strings[id]);
        }

        bool ExerciseEditorValidator::isFirstOldExerciseNameValid()
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            return e->name->validateText();
        }

        bool ExerciseEditorValidator::isLastOldExerciseNameValid()
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            return e->name->validateText();
        }

        bool ExerciseEditorValidator::isFirstOldExerciseURLValid()
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            return e->url->validateText();
        }

        bool ExerciseEditorValidator::isLastOldExerciseURLValid()
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            return e->url->validateText();
        }

        void ExerciseEditorValidator::connectToDownloadSignalsOfItems()
        {
            connect(ee,SIGNAL(exerciseDownloaded(ExerciseItem*)),this,SLOT(saveDemandFromDownloadClicked(ExerciseItem*)));
        }

        void ExerciseEditorValidator::pushDownloadAllButton()
        {
            ee->download_exercises_button->click();
        }

        void ExerciseEditorValidator::pushFirstDeleteButtonOldExercises()
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            e->delete_item->click();
        }

        void ExerciseEditorValidator::pushFirstDeleteButtonNewExercises()
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            e->delete_item->click();
        }

        void ExerciseEditorValidator::saveDemandFromDownloadClicked(ExerciseItem* e)
        {
            last_demand = std::shared_ptr<GUI::DownloadExerciseDemand>(ee->generateDownloadExerciseDemand(e));
        }
    }
}
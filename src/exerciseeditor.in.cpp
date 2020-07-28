#include "include/exerciseeditor.h"
#include <iostream>
namespace FreeFit
{
    namespace GUI
    {
        ToggleableLabel::ToggleableLabel(QString text, QWidget* parent):QLabel(text,parent)
        {
            css_string = QString("background-color:white; color:red; border:2px solid red; border-radius:5px;");
            setSelectable(true);
            setStyleSheet(css_string);
        }

        void ToggleableLabel::mousePressEvent(QMouseEvent* ev)
        {
            emit clicked();
            QLabel::mousePressEvent(ev);
        }

        void ToggleableLabel::setSelectable(bool s)
        {
            if (s)
                connect(this,&ToggleableLabel::clicked,this,&ToggleableLabel::clicked_impl);
            else
                disconnect(this,&ToggleableLabel::clicked,this,&ToggleableLabel::clicked_impl);
        }

        void ToggleableLabel::clicked_impl()
        {
            toggled = !toggled;
            if(toggled)
                css_string.replace("background-color:white; color:red;","background-color:red; color:white;");
            else
                css_string.replace("background-color:red; color:white;","background-color:white; color:red;");
            setStyleSheet(css_string);
        }

        ExerciseItem::ExerciseItem(QWidget* parent):QWidget(parent),muscle_definitions(),default_color("black")
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
/*
            name_label = new QLabel("Name:",this);
            url_label = new QLabel("Youtube-URL:",this);
            start_time_label = new QLabel("Start Time:",this);
            stop_time_label = new QLabel("Stop Time:",this);
*/
            processMovie = new QMovie("${CMAKE_SOURCE_DIR}/tools/loading.gif");
            processLabel = new QLabel(this);
            processLabel->setMovie(processMovie);
            processLabel->hide();
/*
            name_label->setToolTip("No special characters allowed.");
            url_label->setToolTip("Youtube-links only.");
            start_time_label->setToolTip("Enter start of video in format MM:SS.");
            stop_time_label->setToolTip("Enter end of video in format MM:SS.");
*/
            name        = new MaterialTextField("Name",this);
            url         = new MaterialTextField("URL",this);
            start_time  = new MaterialTextField("Start [mm:ss]",this);
            stop_time   = new MaterialTextField("Stop [mm:ss]",this);

            name->setToolTip("No special characters allowed.");
            url->setToolTip("Youtube-links only.");
            start_time->setToolTip("Enter start of video in format MM:SS.");
            stop_time->setToolTip("Enter end of video in format MM:SS.");

            const int edit_line_width = 180;
            const int edit_label_and_line_height = 18;

/*
            name_label->setFixedHeight(edit_label_and_line_height);
            url_label->setFixedHeight(edit_label_and_line_height);
            start_time_label->setFixedHeight(edit_label_and_line_height);
            stop_time_label->setFixedHeight(edit_label_and_line_height);

            name->setFixedSize(edit_line_width,edit_label_and_line_height);
            url->setFixedSize(edit_line_width,edit_label_and_line_height);
            start_time->setFixedSize(edit_line_width,edit_label_and_line_height);
            stop_time->setFixedSize(edit_line_width,edit_label_and_line_height);
*/
            std::regex name_regex("[a-zA-Z0-9\\s]{1,256}");
            auto func_name_regex = [name_regex](std::string s)->bool{return std::regex_match(s,name_regex);};
            name->setValidationFunction(func_name_regex);

            std::regex url_regex("https:\\/\\/www\\.youtube\\.com\\/watch?.*");
            auto func_url_regex = [url_regex](std::string s)->bool{return std::regex_match(s,url_regex);};
            url->setValidationFunction(func_url_regex);

            std::regex int_range_regex("[0-9]{2}:[0-9]{2}");
            auto func_int_regex = [int_range_regex](std::string s)->bool{return std::regex_match(s,int_range_regex);};
            start_time->setValidationFunction(func_int_regex);
            stop_time->setValidationFunction(func_int_regex);

            name->validateText();
            url->validateText();
            start_time->validateText();
            stop_time->validateText();
            
            connect(name,&QLineEdit::textChanged,this,&ExerciseItem::itemChanged);
            connect(url,&QLineEdit::textChanged,this,&ExerciseItem::itemChanged);
            connect(start_time,&QLineEdit::textChanged,this,&ExerciseItem::itemChanged);
            connect(stop_time,&QLineEdit::textChanged,this,&ExerciseItem::itemChanged);

            connect(url,&QLineEdit::textChanged,this,&ExerciseItem::urlChanged);

            item_downloaded_icon = new QLabel("",this);
            item_downloaded_text = new QLabel("",this);
            delete_item = new MaterialButton("Delete",this);
            connect(delete_item,&QPushButton::clicked,this,&ExerciseItem::deleteClicked);

            int row_counter = -1;
            int col_counter = -1;
/*
            ly->addWidget(name_label,++row_counter,++col_counter);
            ly->addWidget(url_label,++row_counter,col_counter);
            ly->addWidget(start_time_label,++row_counter,col_counter);
            ly->addWidget(stop_time_label,++row_counter,col_counter);
*/
            row_counter = -1;
            ly->addWidget(name,++row_counter,++col_counter);
            ly->addWidget(url,++row_counter,col_counter);
            ly->addWidget(start_time,++row_counter,col_counter);
            ly->addWidget(stop_time,++row_counter,col_counter);

            ly->addWidget(delete_item,0,++col_counter,row_counter+1,1,Qt::AlignCenter);
            ly->addWidget(processLabel,0,++col_counter,row_counter+1,1,Qt::AlignCenter);
            ly->addWidget(item_downloaded_text,0,++col_counter,row_counter+1,1,Qt::AlignCenter);
            ly->addWidget(item_downloaded_icon,0,++col_counter,row_counter+1,1,Qt::AlignCenter);

            ly->addWidget(hashtag_widget,++row_counter,0,col_counter,2,Qt::AlignLeft);
        }

        std::list<std::string> ExerciseItem::getMuscleAreas()
        {
            std::list<std::string> l;
            for (auto label : hashtag_labels)
                if(label->isToggled())
                    l.push_back(label->text().remove(QChar('#')).toStdString());
            return l;
        }

        void ExerciseItem::setMuscleAreas(std::set<FreeFit::Data::MuscleGroup> muscles)
        {
            for (auto h : hashtag_labels)
            {
                std::string h_name = h->text().remove(QChar('#')).toStdString();
                for (auto  m : muscles)
                    if (h_name == muscle_definitions.strings[m])
                        h->click();
            }
        }

        bool ExerciseItem::inputIsValid()
        {
            return (name->validateText() && url->validateText() && start_time->validateText() && stop_time->validateText());
        }

        void ExerciseItem::highlightAsFaulty()
        {
            this->setStyleSheet("background-color:red;");
            connect(name,&QLineEdit::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            connect(url,&QLineEdit::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            connect(start_time,&QLineEdit::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            connect(stop_time,&QLineEdit::textChanged,this,&ExerciseItem::resetStylesheetOnce);
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

        void ExerciseItem::resetStylesheetOnce()
        {
            setDefaultBackground();
            disconnect(name,&QLineEdit::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            disconnect(url,&QLineEdit::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            disconnect(start_time,&QLineEdit::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            disconnect(stop_time,&QLineEdit::textChanged,this,&ExerciseItem::resetStylesheetOnce);
        }

        void ExerciseItem::itemChanged()
        {
            unchanged = false;
            disconnect(name,&QLineEdit::textChanged,this,&ExerciseItem::itemChanged);
            disconnect(url,&QLineEdit::textChanged,this,&ExerciseItem::itemChanged);
            disconnect(start_time,&QLineEdit::textChanged,this,&ExerciseItem::itemChanged);
            disconnect(stop_time,&QLineEdit::textChanged,this,&ExerciseItem::itemChanged);
        }

        void ExerciseItem::urlChanged()
        {
            if(url->validateText())
                emit urlChange(url->text().toStdString());
        }

        void ExerciseItem::paintEvent(QPaintEvent* e)
        {
            QStyleOption opt;
            opt.init(this);
            QPainter p(this);
            style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
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
            :   MaterialDialog(),p(t_p),demand_handler(),info_demand_handler(),
                r(t_p.getPathToExerciseDB()),w(t_p.getPathToExerciseDB())
        {                
            ly = new QGridLayout(this);

            add_button = new MaterialButton("Add Exercise",this);
            connect(add_button,&QPushButton::clicked,this,&ExerciseEditor::addExercise);

            new_exercise_label = new QLabel("New Exercises",this);

            new_exercise_area = new QWidget(this);
            new_exercise_area_ly = new QVBoxLayout(new_exercise_area);
            new_exercise_area_ly->addStretch();
            new_exercise_area->setLayout(new_exercise_area_ly);

            new_exercise_scroll_area = new QScrollArea(this);
            new_exercise_scroll_area->setWidget(new_exercise_area);
            new_exercise_scroll_area->setAlignment(Qt::AlignTop);
            new_exercise_scroll_area->setWidgetResizable(true);

            download_exercises_button = new MaterialButton("Add to Exercises",this);
            connect(download_exercises_button,&QPushButton::clicked,this,&ExerciseEditor::downloadAllExercises);

            old_exercise_label = new QLabel("Existing Exercises",this);

            old_exercise_area = new QWidget(this);
            old_exercise_area_ly = new QVBoxLayout(old_exercise_area);
            old_exercise_area_ly->addStretch();
            old_exercise_area->setLayout(old_exercise_area_ly);

            old_exercise_scroll_area = new QScrollArea(this);
            old_exercise_scroll_area->setWidget(old_exercise_area);
            old_exercise_scroll_area->setAlignment(Qt::AlignTop);
            old_exercise_scroll_area->setWidgetResizable(true);

            r.read();
            for (auto e_data : r.getExerciseList())
                addExistingExercise(e_data);

            if (exercise_items.empty())
                addExercise();

            browser = new ExerciseEditorBrowser(this);

            next_page_button = new ControlButton("Workout Type", ControlButton::ForwardButton,ControlButton::Primary,this);
            connect(next_page_button, &QPushButton::clicked, this, &QDialog::accept);

            previous_page_button = new ControlButton("Profile", ControlButton::BackwardButton,ControlButton::Primary,this);
            connect(previous_page_button, &QPushButton::clicked, this, &QDialog::reject);

            QGridLayout* controls_layout = new QGridLayout;
            QSpacerItem* horizontal_spacer = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
            controls_layout->addWidget(previous_page_button,0,0);
            controls_layout->addItem(horizontal_spacer,0,1);
            controls_layout->addWidget(next_page_button,0,2);

            ly->addWidget(add_button,0,1);
            ly->addWidget(new_exercise_label,1,1,1,2);
            ly->addWidget(new_exercise_scroll_area,2,1,1,2);
            ly->addWidget(download_exercises_button,3,1,1,2);
            ly->addWidget(old_exercise_label,4,1,1,2);
            ly->addWidget(old_exercise_scroll_area,5,1,1,2);
            ly->addWidget(browser,0,0,6,1);
            ly->addLayout(controls_layout,6,0,1,3);

            this->setLayout(ly);
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
            std::string mins = s.substr(0,1);
            std::string secs = s.substr(3,4);
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
            new_exercise_items.push_back(e);
            new_exercise_area_ly->insertWidget(0,e);
            connect(e,&ExerciseItem::deleteItemTriggered,this,&ExerciseEditor::deleteExercise);
            connect(e,&ExerciseItem::downloadItemTriggered,this,&ExerciseEditor::downloadExercise);
            connect(this,SIGNAL(exerciseDownloaded(ExerciseItem*)),this,SLOT(moveExerciseToExisting(ExerciseItem*)));
            connect(e,SIGNAL(urlChange(std::string)),this,SLOT(exerciseUrlChanged(std::string)));
            connect(this,SIGNAL(setExerciseStartTimeSignal(std::string)),e,SLOT(setVideoStartTime(std::string)));
            connect(this,SIGNAL(setExerciseEndTimeSignal(std::string)),e,SLOT(setVideoEndTime(std::string)));
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

        void ExerciseEditor::exerciseUrlChanged(std::string url)
        {
            std::shared_ptr<InfoExerciseDemand> d = std::make_shared<InfoExerciseDemand>();
            d->video_url = url;
            int video_length = info_demand_handler.executeDemand(d);
            emit setExerciseEndTimeSignal(secondsIntToTimeFormatString(video_length));
            emit setExerciseStartTimeSignal(secondsIntToTimeFormatString(0));
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
                auto f = [this,e]
                {
                    FreeFit::Data::Exercise e_dat = demand_handler.executeDemand(generateDownloadExerciseDemand(e));
                    e->setVideoPath(e_dat.getVideoPath());
                    e->setThumbnailPath(e_dat.getThumbnailPath());
                    emit exerciseDownloaded(e);
                };
                e->showWaitingSymbol();
                std::thread t = std::thread(f);
                t.detach();
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
            for(auto e : new_exercise_items)
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
//            e->name->textMessageBecauseFocusLost(e->name->text());
        }

        void ExerciseEditorValidator::setFirstNewExerciseURLText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            e->url->setText(QString::fromStdString(s));
//            e->url->textMessageBecauseFocusLost(e->url->text());
        }

        void ExerciseEditorValidator::setFirstNewExerciseStartTimeText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            e->start_time->setText(QString::fromStdString(s));
//            e->start_time->textMessageBecauseFocusLost(e->start_time->text());
        }

        void ExerciseEditorValidator::setFirstNewExerciseStopTimeText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            e->stop_time->setText(QString::fromStdString(s));
//            e->stop_time->textMessageBecauseFocusLost(e->stop_time->text());
        }

        void ExerciseEditorValidator::setFirstNewExerciseMuscleArea(int id)
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            e->hashtag_labels[id]->clicked();
        }

        void ExerciseEditorValidator::setLastNewExerciseNameText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            e->name->setText(QString::fromStdString(s));
//            e->name->textMessageBecauseFocusLost(e->name->text());
        }

        void ExerciseEditorValidator::setLastNewExerciseURLText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            e->url->setText(QString::fromStdString(s));
//            e->url->textMessageBecauseFocusLost(e->url->text());
        }

        void ExerciseEditorValidator::setLastNewExerciseStartTimeText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            e->start_time->setText(QString::fromStdString(s));
//            e->start_time->textMessageBecauseFocusLost(e->start_time->text());
        }

        void ExerciseEditorValidator::setLastNewExerciseStopTimeText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            e->stop_time->setText(QString::fromStdString(s));
//            e->stop_time->textMessageBecauseFocusLost(e->stop_time->text());
        }

        void ExerciseEditorValidator::setLastNewExerciseMuscleArea(int id)
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            e->hashtag_labels[id]->clicked();
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

        bool ExerciseEditorValidator::isFirstNewExerciseStartTimeValid()
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            return e->start_time->validateText();
        }

        bool ExerciseEditorValidator::isFirstNewExerciseStopTimeValid()
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            return e->stop_time->validateText();
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
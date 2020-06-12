#include "include/exerciseeditor.h"

namespace FreeFit
{
    namespace GUI
    {
        void ToggleableLabel::mousePressEvent(QMouseEvent* ev)
        {
            this->clicked();
            QLabel::mousePressEvent(ev);
        }

        void ToggleableLabel::clicked()
        {
            toggled = !toggled;
            if(toggled)
                this->setStyleSheet("border-color:blue;color:blue;" );
            else
                this->setStyleSheet("");
        }

        void ClickableLabel::mousePressEvent(QMouseEvent* ev)
        {
            emit labelClicked();
            QLabel::mousePressEvent(ev);
        }

        void WriteableLine::focusOutEvent(QFocusEvent* ev)
        {
            this->deselect();
            emit textMessageBecauseFocusLost(this->text());
            QLineEdit::focusOutEvent(ev);
        }

        EditableLine::EditableLine(QString text,QWidget* parent):QStackedWidget(parent)
        {
            l = new ClickableLabel(text,this);
            le = new WriteableLine(text,this);
            this->setFocusPolicy(Qt::StrongFocus);
            this->setFocusProxy(le);
            this->addWidget(l);
            this->addWidget(le);
            connect(l,&ClickableLabel::labelClicked,this,&EditableLine::showLineEdit);
            connect(le,&WriteableLine::textMessageBecauseFocusLost,this,&EditableLine::showLabelAndSetText);
            connect(le,&QLineEdit::textChanged,this,&EditableLine::validateText);
            connect(le,&QLineEdit::textEdited,this,&EditableLine::textChanged);
        }

        void EditableLine::setContent(std::string c)
        {
            le->setText(QString::fromStdString(c));
            l->setText(QString::fromStdString(c));
        }

        void EditableLine::showLineEdit()
        {
            le->selectAll();
            this->setCurrentWidget(le);
        }

        void EditableLine::showLabelAndSetText(QString t)
        {
            this->setCurrentWidget(l);
            l->setText(t);
        }

        void EditableLine::styleTextAsOldAndValid()
        {
            l->setStyleSheet("background-color:DarkSeaGreen;");
            le->setStyleSheet("background-color:DarkSeaGreen;");
        }

        void EditableLine::deactivateTextAndHighlightAsValid()
        {            
            disconnect(l,&ClickableLabel::labelClicked,this,&EditableLine::showLineEdit);
            disconnect(le,&WriteableLine::textMessageBecauseFocusLost,this,&EditableLine::showLabelAndSetText);
            disconnect(le,&QLineEdit::textChanged,this,&EditableLine::validateText);
            disconnect(le,&QLineEdit::textEdited,this,&EditableLine::textChanged);
            this->setCurrentWidget(l);
        }

        bool EditableLine::validateText()
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

            name_label = new QLabel("Name:",this);
            url_label = new QLabel("Youtube-URL:",this);
            start_time_label = new QLabel("Start Time:",this);
            stop_time_label = new QLabel("Stop Time:",this);
            
            name_label->setToolTip("No special characters allowed.");
            url_label->setToolTip("Youtube-links only.");
            start_time_label->setToolTip("Enter start of video in format MM:SS.");
            stop_time_label->setToolTip("Enter end of video in format MM:SS.");

            name        = new EditableLine("...",this);
            url         = new EditableLine("...",this);
            start_time  = new EditableLine("MM:SS",this);
            stop_time   = new EditableLine("MM:SS",this);

            name->setToolTip("No special characters allowed.");
            url->setToolTip("Youtube-links only.");
            start_time->setToolTip("Enter start of video in format MM:SS.");
            stop_time->setToolTip("Enter end of video in format MM:SS.");

            const int edit_line_width = 180;
            const int edit_label_and_line_height = 18;

            name_label->setFixedHeight(edit_label_and_line_height);
            url_label->setFixedHeight(edit_label_and_line_height);
            start_time_label->setFixedHeight(edit_label_and_line_height);
            stop_time_label->setFixedHeight(edit_label_and_line_height);

            name->setFixedSize(edit_line_width,edit_label_and_line_height);
            url->setFixedSize(edit_line_width,edit_label_and_line_height);
            start_time->setFixedSize(edit_line_width,edit_label_and_line_height);
            stop_time->setFixedSize(edit_line_width,edit_label_and_line_height);

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
            
            connect(name,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
            connect(url,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
            connect(start_time,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
            connect(stop_time,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);

            item_downloaded_icon = new QLabel("",this);
            item_downloaded_text = new QLabel("",this);
            delete_item = new QPushButton("Delete",this);
            connect(delete_item,&QPushButton::clicked,this,&ExerciseItem::deleteClicked);

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

        void ExerciseItem::highlightAsOldAndValid()
        {
            this->setStyleSheet("background-color:grey;");
            name->styleTextAsOldAndValid();
            url->deactivateTextAndHighlightAsValid();
            start_time->deactivateTextAndHighlightAsValid();
            stop_time->deactivateTextAndHighlightAsValid();
            item_downloaded_text->setText("Downloaded!");
            //item_downloaded_icon->setPixmap(this->style()->standardIcon(QStyle::SP_DialogApplyButton).pixmap(delete_item->size().height()));
        }

        void ExerciseItem::highlightAsFaulty()
        {
            this->setStyleSheet("background-color:red;");
            connect(name,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            connect(url,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            connect(start_time,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            connect(stop_time,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
        }

        void ExerciseItem::setDefaultBackground()
        {
            this->setStyleSheet("background-color:" + QString::fromStdString(default_color) + ";");
        }

        void ExerciseItem::resetStylesheetOnce()
        {
            setDefaultBackground();
            disconnect(name,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            disconnect(url,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            disconnect(start_time,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
            disconnect(stop_time,&EditableLine::textChanged,this,&ExerciseItem::resetStylesheetOnce);
        }

        void ExerciseItem::itemChanged()
        {
            unchanged = false;
            disconnect(name,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
            disconnect(url,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
            disconnect(start_time,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
            disconnect(stop_time,&EditableLine::textChanged,this,&ExerciseItem::itemChanged);
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
            address = new QLineEdit(start_url,this);

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
            :   p(t_p),demand_handler(),
                r(t_p.getPathToExerciseDB()),w(t_p.getPathToExerciseDB())
        {                
            ly = new QGridLayout(this);

            button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            
            connect(button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
            connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

            add_button = new QPushButton("Add Exercise",this);
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

            download_exercises_button = new QPushButton("Add to Exercises",this);
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

            ly->addWidget(add_button,0,1);
            ly->addWidget(new_exercise_label,1,1,1,2);
            ly->addWidget(new_exercise_scroll_area,2,1,1,2);
            ly->addWidget(download_exercises_button,3,1,1,2);
            ly->addWidget(old_exercise_label,4,1,1,2);
            ly->addWidget(old_exercise_scroll_area,5,1,1,2);
            ly->addWidget(button_box,6,1,1,2);
            ly->addWidget(browser,0,0,7,1);

            this->setLayout(ly);
        }

        void ExerciseEditor::accept()
        {   
            std::list<FreeFit::Data::Exercise> lst;
            for (auto e : exercise_items)
                lst.push_back(exerciseItemToData(e));
            w.createNodeTree(lst);
            w.write();
            QDialog::accept();
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
            e->highlightAsOldAndValid();
            old_exercise_area_ly->insertWidget(0,e);
            connect(e,&ExerciseItem::deleteItemTriggered,this,&ExerciseEditor::deleteExercise);
            connect(e,&ExerciseItem::downloadItemTriggered,this,&ExerciseEditor::downloadExercise);
            exercise_items.push_back(e);
            repaintExerciseBackgrounds();
        }

        void ExerciseEditor::addExercise()
        {
            ExerciseItem* e = new ExerciseItem(this);
            new_exercise_items.push_back(e);
            new_exercise_area_ly->insertWidget(0,e);
            connect(e,&ExerciseItem::deleteItemTriggered,this,&ExerciseEditor::deleteExercise);
            connect(e,&ExerciseItem::downloadItemTriggered,this,&ExerciseEditor::downloadExercise);
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

        void ExerciseEditor::repaintExerciseBackgrounds()
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

        bool ExerciseEditor::downloadExercise(ExerciseItem* e)
        {
            if(e->inputIsValid())
            {
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
            std::list<ExerciseItem*> to_delete;
            for(auto e : new_exercise_items)
                if(downloadExercise(e))
                    to_delete.push_back(e);
            
            for(auto e : to_delete)
            {
                registerExerciseItem(e);
                new_exercise_items.remove(e);
            }                
        }

        void ExerciseEditor::deleteExercise(ExerciseItem* e)
        {
            exercise_items.remove(e);
            new_exercise_items.remove(e);
            new_exercise_area_ly->removeWidget(e);
            disconnect(e,nullptr,nullptr,nullptr);
            delete e;
            repaintExerciseBackgrounds();
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
            e->name->le->setText(QString::fromStdString(s));
            e->name->le->textMessageBecauseFocusLost(e->name->le->text());
        }

        void ExerciseEditorValidator::setFirstNewExerciseURLText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            e->url->le->setText(QString::fromStdString(s));
            e->url->le->textMessageBecauseFocusLost(e->url->le->text());
        }

        void ExerciseEditorValidator::setFirstNewExerciseStartTimeText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            e->start_time->le->setText(QString::fromStdString(s));
            e->start_time->le->textMessageBecauseFocusLost(e->start_time->le->text());
        }

        void ExerciseEditorValidator::setFirstNewExerciseStopTimeText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            e->stop_time->le->setText(QString::fromStdString(s));
            e->stop_time->le->textMessageBecauseFocusLost(e->stop_time->le->text());
        }

        void ExerciseEditorValidator::setFirstNewExerciseMuscleArea(int id)
        {
            ExerciseItem* e = *(ee->new_exercise_items.begin());
            e->hashtag_labels[id]->clicked();
        }

        void ExerciseEditorValidator::setLastNewExerciseNameText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            e->name->le->setText(QString::fromStdString(s));
            e->name->le->textMessageBecauseFocusLost(e->name->le->text());
        }

        void ExerciseEditorValidator::setLastNewExerciseURLText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            e->url->le->setText(QString::fromStdString(s));
            e->url->le->textMessageBecauseFocusLost(e->url->le->text());
        }

        void ExerciseEditorValidator::setLastNewExerciseStartTimeText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            e->start_time->le->setText(QString::fromStdString(s));
            e->start_time->le->textMessageBecauseFocusLost(e->start_time->le->text());
        }

        void ExerciseEditorValidator::setLastNewExerciseStopTimeText(std::string s)
        {
            ExerciseItem* e = *(ee->new_exercise_items.rbegin());
            e->stop_time->le->setText(QString::fromStdString(s));
            e->stop_time->le->textMessageBecauseFocusLost(e->stop_time->le->text());
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
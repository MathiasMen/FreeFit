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
            start_time_label->setToolTip("Enter seconds between 0 and 999.");
            stop_time_label->setToolTip("Enter seconds between 0 and 999.");

            name        = new EditableLine("...",this);
            url         = new EditableLine("...",this);
            start_time  = new EditableLine("...",this);
            stop_time   = new EditableLine("...",this);

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
            url->styleTextAsOldAndValid();
            start_time->styleTextAsOldAndValid();
            stop_time->styleTextAsOldAndValid();
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
            download_all_button = new QPushButton("Download All",this);
            connect(add_button,&QPushButton::clicked,this,&ExerciseEditor::addExercise);
            connect(download_all_button,&QPushButton::clicked,this,&ExerciseEditor::downloadAllExercises);

            exercise_area = new QWidget(this);
            exercise_area_ly = new QVBoxLayout(exercise_area);
            exercise_area_ly->addStretch();
            exercise_area->setLayout(exercise_area_ly);

            scroll_area = new QScrollArea(this);
            scroll_area->setWidget(exercise_area);
            scroll_area->setAlignment(Qt::AlignTop);
            scroll_area->setWidgetResizable(true);

            r.read();
            for (auto e_data : r.getExerciseList())
                addExistingExercise(e_data);

            if (exercise_items.empty())
                addExercise();

            browser = new ExerciseEditorBrowser(this);

            ly->addWidget(browser,0,0,5,10);
            ly->addWidget(add_button,0,10,1,1);
            ly->addWidget(download_all_button,0,13,1,1);
            ly->addWidget(scroll_area,1,10,4,4);
            ly->addWidget(button_box,5,7);
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
            e_dat.setVideoStartTime(e->getVideoStartTime());
            e_dat.setVideoEndTime(e->getVideoEndTime());
            for (auto m : e->getMuscleAreas())
                e_dat.addTrainedMuscle(FreeFit::Data::stringToMuscleGroup(m));

            return e_dat;
        }

        void ExerciseEditor::registerExerciseItem(ExerciseItem* e)
        {
            exercise_area_ly->insertWidget(0,e);
            connect(e,&ExerciseItem::deleteItemTriggered,this,&ExerciseEditor::deleteExercise);
            connect(e,&ExerciseItem::downloadItemTriggered,this,&ExerciseEditor::downloadExercise);
            exercise_items.push_back(e);
            repaintExerciseBackgrounds();
        }

        void ExerciseEditor::addExercise()
        {
            ExerciseItem* e = new ExerciseItem(this);
            registerExerciseItem(e);
        }

        void ExerciseEditor::addExistingExercise(FreeFit::Data::Exercise e_dat)
        {
            ExerciseItem* e = new ExerciseItem(this);
            e->setName(e_dat.getName());
            e->setVideoPath(e_dat.getVideoPath());
            e->setThumbnailPath(e_dat.getThumbnailPath());
            e->setURL(e_dat.getVideoURL());
            e->setMuscleAreas(e_dat.getTrainedMuscles());
            e->setVideoStartTime(e_dat.getVideoStartTime());
            e->setVideoEndTime(e_dat.getVideoEndTime());

            registerExerciseItem(e);
            e->highlightAsOldAndValid();
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
            d->video_start_time = e->getVideoStartTime();
            d->video_end_time = e->getVideoEndTime();
            d->muscle_areas = e->getMuscleAreas();
            return d;
        }

        void ExerciseEditor::downloadExercise(ExerciseItem* e)
        {
            if(e->inputIsValid())
            {
                FreeFit::Data::Exercise e_dat = demand_handler.executeDemand(generateDownloadExerciseDemand(e));
                e->setVideoPath(e_dat.getVideoPath());
                e->setThumbnailPath(e_dat.getThumbnailPath());
            }
            else
                e->highlightAsFaulty();
        }

        void ExerciseEditor::downloadAllExercises()
        {
            for(auto e : exercise_items)
                downloadExercise(e);
        }

        void ExerciseEditor::deleteExercise(ExerciseItem* e)
        {
            exercise_items.remove(e);
            exercise_area_ly->removeWidget(e);
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
            return ee->generateDownloadExerciseDemand(*(ee->exercise_items.begin()));
        }

        void ExerciseEditorValidator::setFirstExerciseNameText(std::string s)
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            e->name->le->setText(QString::fromStdString(s));
            e->name->le->textMessageBecauseFocusLost(e->name->le->text());
        }

        void ExerciseEditorValidator::setFirstExerciseURLText(std::string s)
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            e->url->le->setText(QString::fromStdString(s));
            e->url->le->textMessageBecauseFocusLost(e->url->le->text());
        }

        void ExerciseEditorValidator::setFirstExerciseStartTimeText(std::string s)
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            e->start_time->le->setText(QString::fromStdString(s));
            e->start_time->le->textMessageBecauseFocusLost(e->start_time->le->text());
        }

        void ExerciseEditorValidator::setFirstExerciseStopTimeText(std::string s)
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            e->stop_time->le->setText(QString::fromStdString(s));
            e->stop_time->le->textMessageBecauseFocusLost(e->stop_time->le->text());
        }

        void ExerciseEditorValidator::setFirstExerciseMuscleArea(int id)
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            e->hashtag_labels[id]->clicked();
        }

        void ExerciseEditorValidator::setLastExerciseNameText(std::string s)
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            e->name->le->setText(QString::fromStdString(s));
            e->name->le->textMessageBecauseFocusLost(e->name->le->text());
        }

        void ExerciseEditorValidator::setLastExerciseURLText(std::string s)
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            e->url->le->setText(QString::fromStdString(s));
            e->url->le->textMessageBecauseFocusLost(e->url->le->text());
        }

        void ExerciseEditorValidator::setLastExerciseStartTimeText(std::string s)
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            e->start_time->le->setText(QString::fromStdString(s));
            e->start_time->le->textMessageBecauseFocusLost(e->start_time->le->text());
        }

        void ExerciseEditorValidator::setLastExerciseStopTimeText(std::string s)
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            e->stop_time->le->setText(QString::fromStdString(s));
            e->stop_time->le->textMessageBecauseFocusLost(e->stop_time->le->text());
        }

        void ExerciseEditorValidator::setLastExerciseMuscleArea(int id)
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            e->hashtag_labels[id]->clicked();
        }

        bool ExerciseEditorValidator::isFirstExerciseNameValid()
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            return e->name->validateText();
        }

        bool ExerciseEditorValidator::isFirstExerciseURLValid()
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            return e->url->validateText();
        }

        bool ExerciseEditorValidator::isFirstExerciseStartTimeValid()
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            return e->start_time->validateText();
        }

        bool ExerciseEditorValidator::isFirstExerciseStopTimeValid()
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            return e->stop_time->validateText();
        }

        void ExerciseEditorValidator::connectToDownloadSignalsOfItems()
        {
            for (auto e : ee->exercise_items)
                connect(e,&ExerciseItem::downloadItemTriggered,this,&ExerciseEditorValidator::saveDemandFromDownloadClicked);
        }

        void ExerciseEditorValidator::pushFirstDownloadButton()
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            e->download_item->click();
        }

        void ExerciseEditorValidator::pushLastDownloadButton()
        {
            ExerciseItem* e = *(ee->exercise_items.rbegin());
            e->download_item->click();
        }

        void ExerciseEditorValidator::pushFirstDeleteButton()
        {
            ExerciseItem* e = *(ee->exercise_items.begin());
            e->delete_item->click();
        }

        void ExerciseEditorValidator::saveDemandFromDownloadClicked(ExerciseItem* e)
        {
            last_demand = std::shared_ptr<GUI::DownloadExerciseDemand>(ee->generateDownloadExerciseDemand(e));
        }

    }
}
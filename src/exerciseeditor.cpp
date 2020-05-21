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
            emit textMessageBecauseFocusLost(this->text());
            QLineEdit::focusOutEvent(ev);
        }

        EditableLine::EditableLine(QString text,QWidget* parent):QStackedWidget(parent)
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

        void EditableLine::setContent(std::string c)
        {
            le->setText(QString::fromStdString(c));
            l->setText(QString::fromStdString(c));
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

    }
}
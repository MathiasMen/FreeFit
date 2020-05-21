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
    }
}
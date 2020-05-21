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
    }
}
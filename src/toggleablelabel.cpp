#include "include/toggleablelabel.h"

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
    }
}

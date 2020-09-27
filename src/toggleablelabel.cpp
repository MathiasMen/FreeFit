#include "include/toggleablelabel.h"

namespace FreeFit
{
    namespace GUI
    {    
        ToggleableLabel::ToggleableLabel(QString text, QWidget* parent):QLabel(text,parent),toggled(false),color("#ff0000")
        {
            updateCSS();
            setSelectable(true);
        }

        void ToggleableLabel::updateCSS()
        {
            if (toggled)
                css_string = QString::fromStdString("background-color:" + color + "; color:white; border:2px solid " + color + "; border-radius:5px;");
            else
                css_string = QString::fromStdString("background-color:white; color:" + color + "; border:2px solid " + color + "; border-radius:5px;");
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

        void ToggleableLabel::setColor(std::string c)
        {
            color = c;
            updateCSS();
        }

        void ToggleableLabel::clicked_impl()
        {
            toggled = !toggled;
            updateCSS();
        }
    }
}

#include "include/materialclip.h"

namespace FreeFit
{
    namespace GUI
    {
        MaterialClip::MaterialClip(QString t, QWidget* parent) : QLabel(parent)
        {
            setFixedSize(50,50);
            setAlignment(Qt::AlignCenter);
            setInitials(t);
            setToolTip(t);
            updateStyle();
        }

        void MaterialClip::select()
        {
            selected = true;
            updateStyle();
        }

        void MaterialClip::deselect()
        {
            selected = false;
            updateStyle();
        }

        void MaterialClip::setInitials(QString s)
        {
            setText(s.at(0));
        }

        void MaterialClip::mousePressEvent(QMouseEvent* ev)
        {
            selected = !selected;
            updateStyle();
            emit clicked(this);
        }
    
        void MaterialClip::updateStyle()
        {
            if (selected)
                css_string.replace("background-color:red; color:black","background-color:black; color:red;");
            else
                css_string.replace("background-color:black; color:red","background-color:red; color:black;");
            setStyleSheet(css_string);
        }
    }
}
#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QString>

namespace FreeFit
{
    namespace GUI
    {
        class MaterialClip : public QLabel
        {
        Q_OBJECT
        public:
            MaterialClip(QString t = "", QWidget* parent = nullptr) : QLabel(parent)
            {
                setInitials(t);
                setToolTip(t);
                updateStyle();
            }

            void select()
            {
                selected = true;
                updateStyle();
            }

            void deselect()
            {
                selected = false;
                updateStyle();
            }

            void setInitials(QString s)
            {
                setText(s.at(0));
            }
        protected:
            void mousePressEvent(QMouseEvent* ev) override
            {
                selected = !selected;
                updateStyle();
                emit clicked(this);
            }

        signals:
            void clicked(MaterialClip*);
        
        private:
            void updateStyle()
            {
                if (selected)
                    css_string.replace("background-color:red; color:black","background-color:black; color:red;");
                else
                    css_string.replace("background-color:black; color:red","background-color:red; color:black;");
                setStyleSheet(css_string);
            }            
            
            bool selected = false;
            QString css_string = "background-color:red; color:black;";

        };
    }
}
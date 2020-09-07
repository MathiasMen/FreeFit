#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

namespace FreeFit
{
    namespace GUI
    {
        class ProfileItem : public QWidget
        {
        public:
            ProfileItem(QString t_name = "new", QWidget* t_parent = nullptr) : QWidget(t_parent)
            {
                const int width = 50;
                const int height = 50;
                this->setFixedSize(width,height);
                ly = new QGridLayout(this);
                name_label = new QLabel(this);
                name_label->setFixedSize(width,height);
                setName(t_name);
            }

            void setName(QString name)
            {
                name_label->setText(name);
            }

        protected:
            void mousePressEvent(QMouseEvent* ev)
            {
                QWidget::mousePressEvent(ev);

                selected = !selected;
                updateStyle();
            }

        private:
            void updateStyle()
            {
                if (selected)
                    css_string.replace("color:grey; border: 2px solid grey;","color:red; border: 2px solid red;");
                else
                    css_string.replace("color:red; border: 2px solid red;","color:grey; border: 2px solid grey;");
                setStyleSheet(css_string);
            }

            QGridLayout* ly;
            QLabel* name_label;

            bool selected = false;
            QString css_string = "color:grey; border: 2px solid grey; text-align:center;";
        };
    }
}
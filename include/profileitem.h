#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include <iostream>

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
                for (unsigned int i = 0; i < 5; i++)
                    decreaseLabelTextSizeIfNecessary(name);
                name = elidedTextIfNecessary(name);
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

            void decreaseLabelTextSizeIfNecessary(QString s)
            {
                QFont font = name_label->font();
                QRect bounds = name_label->rect();
                QRect font_bounds = QFontMetrics(font).boundingRect(s);
                if (font_bounds.width() > bounds.width() - 2 || font_bounds.height() > bounds.height() - 2)
                    font.setPointSize(font.pointSize() - 1);
                name_label->setFont(font);
            }

            QString elidedTextIfNecessary(QString s)
            {
                QFont font = name_label->font();
                QRect bounds = name_label->rect();
                QRect font_bounds = QFontMetrics(font).boundingRect(s);

                if (font_bounds.width() > bounds.width() -2 || font_bounds.height() > bounds.height() - 2)
                    return QFontMetrics(font).elidedText(s,Qt::ElideRight,bounds.width() - 2);
                else
                    return s;
            }


            QGridLayout* ly;
            QLabel* name_label;

            bool selected = false;
            QString css_string = "color:grey; border: 2px solid grey; border-radius:5px; text-align:center;";
        };
    }
}
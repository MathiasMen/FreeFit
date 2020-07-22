#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QGridLayout>

#include <iostream>

namespace FreeFit
{
    namespace GUI
    {
        class MaterialSliderHandle : public QLabel
        {
        public:
            MaterialSliderHandle(QString text, QWidget* parent = nullptr):QLabel(text,parent)
            {
                setFixedSize(10,20);
                setStyleSheet("background-color:black;");
            }
        protected:
            void mousePressEvent(QMouseEvent* e)
            {
                pressed = true;
                offset = e->pos();
            }

            void mouseMoveEvent(QMouseEvent* e)
            {
                if (pressed)
                {
                    QPoint old_pos = this->pos();
                    QPoint new_pos = mapToParent(e->pos() - offset);
                    this->move(new_pos.x(),old_pos.y());
                }
            }

            void mouseReleaseEvent(QMouseEvent* e)
            {
                pressed = false;
                offset = QPoint();
            }
        private:
            QPoint offset;
            bool pressed = false;
        };

        class MaterialSlider : public QWidget
        {
        public:
            MaterialSlider(QWidget* parent = nullptr):QWidget(parent)
            {
                ly = new QGridLayout(this);
                setFixedSize(200,50);
                setStyleSheet("background-color:red;");
                left_handle = new MaterialSliderHandle("",this);
                left_handle->setStyleSheet("background-color:black;");
                ly->addWidget(left_handle);
            }
        private:
            MaterialSliderHandle* left_handle;
            QGridLayout* ly;
        };
    }
}
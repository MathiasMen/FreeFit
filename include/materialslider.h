#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QGridLayout>
#include <QPainter>
#include <QPen>

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

            void setMinX(int x){minVal = x;}
            void setMaxX(int x){maxVal = x;}
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
                    if (new_pos.x() < maxVal && new_pos.x() > minVal)
                        this->move(new_pos.x(),old_pos.y());
                }
            }

            void mouseReleaseEvent(QMouseEvent* e)
            {
                pressed = false;
                offset = QPoint();
            }
        private:
            int minVal;
            int maxVal;
            QPoint offset;
            bool pressed = false;
        };

        class MaterialSlider : public QWidget
        {
        public:
            MaterialSlider(QWidget* parent = nullptr):QWidget(parent)
            {
                setFixedSize(200,40);
                setStyleSheet("background-color:red;");
                
                left_handle = new MaterialSliderHandle("",this);
                right_handle = new MaterialSliderHandle("",this);
                
                left_handle->setMinX(distance_line_to_border - 5);
                left_handle->setMaxX(this->rect().width()-distance_line_to_border - 5);
                right_handle->setMinX(distance_line_to_border - 5);
                right_handle->setMaxX(this->rect().width()-distance_line_to_border - 5);

                left_handle->setStyleSheet("background-color:black;");
                right_handle->setStyleSheet("background-color:black;");

                left_handle->move(left_handle->mapToParent(QPoint(distance_line_to_border - 5, this->rect().height()/2 - 10)));
                right_handle->move(right_handle->mapToParent(QPoint(this->rect().width() - distance_line_to_border - 5, this->rect().height()/2 - 10)));
            }
        protected:
            void paintEvent(QPaintEvent* e) override
            {
                QWidget::paintEvent(e);
                QPen pen;
                pen.setColor(QColor(Qt::black));
                QPainter painter(this);
                painter.setPen(pen);
                painter.drawLine(distance_line_to_border,this->rect().height()/2,this->rect().width()-distance_line_to_border,this->rect().height()/2);
            }
        private:
            const int distance_line_to_border = 20;
            MaterialSliderHandle* left_handle;
            MaterialSliderHandle* right_handle;
        };
    }
}
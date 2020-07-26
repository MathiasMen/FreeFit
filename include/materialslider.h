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
        Q_OBJECT
        public:
            MaterialSliderHandle(QString text, QWidget* parent = nullptr):QLabel(text,parent)
            {
                setFixedSize(10,20);
                setStyleSheet("background-color:black;");
            }

            void setMinX(int x){minVal = x;}
            void setMaxX(int x){maxVal = x;}
        signals:
            void moved(int);
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
                    {
                        this->move(new_pos.x(),old_pos.y());
                        emit moved(new_pos.x());
                    }
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
        Q_OBJECT
        public:
            MaterialSlider(QWidget* parent = nullptr):QWidget(parent)
            {
                setFixedSize(200,40);
                setStyleSheet("background-color:white;");
                
                left_handle = new MaterialSliderHandle("",this);
                right_handle = new MaterialSliderHandle("",this);

                const int half_handle_width = 10/2;
                const int half_handle_height = 20/2;
                
                minPosValue = distance_line_to_border - half_handle_width;
                maxPosValue = this->rect().width() - distance_line_to_border - half_handle_width;

                left_handle->setMinX(minPosValue);
                left_handle->setMaxX(maxPosValue);
                right_handle->setMinX(minPosValue);
                right_handle->setMaxX(maxPosValue);

                connect(left_handle,&MaterialSliderHandle::moved,this,&MaterialSlider::leftHandleMoved);
                connect(right_handle,&MaterialSliderHandle::moved,this,&MaterialSlider::rightHandleMoved);

                left_handle->setStyleSheet("background-color:red;");
                right_handle->setStyleSheet("background-color:red;");
                
                left_handle->move(left_handle->mapToParent(QPoint(minPosValue, this->rect().height()/2 - half_handle_height)));
                right_handle->move(right_handle->mapToParent(QPoint(maxPosValue, this->rect().height()/2 - half_handle_height)));
            }

            void setMinValue(int v){min_mapped_value = v;}
            void setMaxValue(int v){max_mapped_value = v;}
        public slots:
            void leftHandleMoved(int new_x)
            {
                right_handle->setMinX(new_x);
            }

            void rightHandleMoved(int new_x)
            {
                left_handle->setMaxX(new_x);
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
            int minPosValue;
            int maxPosValue;
            int min_mapped_value;
            int max_mapped_value;
            MaterialSliderHandle* left_handle;
            MaterialSliderHandle* right_handle;
        };
    }
}
#include "include/materialslider.h"

namespace FreeFit
{
    namespace GUI
    {
            MaterialSliderHandle::MaterialSliderHandle(int width, int height, QWidget* parent):QLabel(parent)
            {
                setFixedSize(width,height);
                setStyleSheet("background-color:black; border: 0px; margin: 0px; padding: 0px;");
            }

            void MaterialSliderHandle::mousePressEvent(QMouseEvent* e)
            {
                pressed = true;
                offset = e->pos();
            }

            void MaterialSliderHandle::mouseMoveEvent(QMouseEvent* e)
            {
                if (pressed)
                {
                    QPoint old_pos = this->pos();
                    QPoint new_pos = mapToParent(e->pos() - offset);
                    if (new_pos.x() <= maxVal && new_pos.x() >= minVal)
                    {
                        this->move(new_pos.x(),old_pos.y());
                        emit moved(new_pos.x());
                    }
                }
            }

            void MaterialSliderHandle::mouseReleaseEvent(QMouseEvent* e)
            {
                pressed = false;
                offset = QPoint();
            }

            MaterialSlider::MaterialSlider(QWidget* parent,int h_w,int h_h)
                :QWidget(parent), handle_width(h_w), handle_height(h_h)
            {
                setMinimumSize(200,30);
                setStyleSheet("background-color:white;");
                
                left_handle = new MaterialSliderHandle(handle_width,handle_height,this);
                right_handle = new MaterialSliderHandle(handle_width,handle_height,this);

                minPosValue = distance_line_to_border - handle_width/2;
                maxPosValue = this->rect().width() - distance_line_to_border - handle_width/2;
                min_mapped_value = 0.0;
                max_mapped_value = 1.0;

                left_handle->setMinX(minPosValue);
                left_handle->setMaxX(maxPosValue);
                right_handle->setMinX(minPosValue);
                right_handle->setMaxX(maxPosValue);
                updateMappedValues();

                connect(left_handle,&MaterialSliderHandle::moved,this,&MaterialSlider::leftHandleMoved);
                connect(right_handle,&MaterialSliderHandle::moved,this,&MaterialSlider::rightHandleMoved);

                left_handle->setStyleSheet("background-color:red;");
                right_handle->setStyleSheet("background-color:red;");
                
                left_handle->move(left_handle->mapToParent(QPoint(minPosValue, this->rect().height()/2 - handle_height/2)));
                right_handle->move(right_handle->mapToParent(QPoint(maxPosValue, this->rect().height()/2 - handle_height/2)));
            }

            void MaterialSlider::leftHandleMoved(int new_x)
            {
                right_handle->setMinX(new_x);
                minPosValue = new_x;
                updateMappedValues();
            }

            void MaterialSlider::rightHandleMoved(int new_x)
            {
                left_handle->setMaxX(new_x);
                maxPosValue = new_x;
                updateMappedValues();
            }

            void MaterialSlider::paintEvent(QPaintEvent* e)
            {
                QWidget::paintEvent(e);
                QPen pen;
                pen.setColor(QColor(Qt::black));
                QPainter painter(this);
                painter.setPen(pen);
                QPoint start = {distance_line_to_border,this->rect().height()/2};
                int line_length = lineEndPosX() - lineStartPosX();
                painter.drawLine(start.x(),start.y(),start.x()+line_length,start.y());
            }

            void MaterialSlider::updateMappedValues()
            {
                int line_length = lineEndPosX() - lineStartPosX();
                int x_lower = minPosValue - lineStartPosX();
                int x_upper = maxPosValue - lineStartPosX();
                double dx = (max_mapped_value - min_mapped_value)/line_length;
                lower_mapped_value = x_lower*dx;
                upper_mapped_value = x_upper*dx;
                emit valuesChanged(lower_mapped_value,upper_mapped_value);
            }

            int MaterialSlider::lineStartPosX()
            {
                return distance_line_to_border - handle_width/2;
            }

            int MaterialSlider::lineEndPosX()
            {
                return (this->rect().width() - distance_line_to_border - handle_width/2);
            }
    }
}
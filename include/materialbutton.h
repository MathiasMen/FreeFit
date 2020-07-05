#pragma once

#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

#include <iostream>
#include <functional>

namespace FreeFit
{
    namespace GUI
    {
        class MaterialButton : public QPushButton
        {
            public:
                MaterialButton(QString t, QWidget* parent = nullptr) : QPushButton(t,parent)
                {
                    this->setStyleSheet("background-color:white; color:black; border: 2px solid red; padding: 0px;");
                    this->setFlat(true);
                }
            protected:
                void paintEvent(QPaintEvent* ev) override
                {
                    QPushButton::paintEvent(ev);
                    QPainter painter(this);

                    if(currentPaintFunction)
                        currentPaintFunction(&painter);
                }

                void mousePressEvent(QMouseEvent* ev) override
                {
                    click_pos = ev->localPos();
                    currentPaintFunction = std::bind(&MaterialButton::animateRipple,this,std::placeholders::_1);
                    repaint();
                    QPushButton::mousePressEvent(ev);
                }

            private:
                void animateRipple(QPainter* painter)
                {
                    QPen pen = painter->pen();
                    pen.setColor(Qt::red);
                    painter->setPen(pen);
                    painter->drawEllipse(click_pos,10,10);
                }

                QPointF click_pos;
                std::function<void(QPainter*)> currentPaintFunction;
        };
    }
}
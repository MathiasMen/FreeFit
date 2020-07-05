#pragma once

#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QRadialGradient>

#include <iostream>
#include <functional>

namespace FreeFit
{
    namespace GUI
    {
        class MaterialButton : public QPushButton
        {
            Q_OBJECT
            public:
                MaterialButton(QString t, QWidget* parent = nullptr) : QPushButton(t,parent),t_update(new QTimer)
                {
                    this->setStyleSheet("background-color:white; color:black; border: 2px solid red; padding: 20px;");
                    this->setFlat(true);
                    connect(this,SIGNAL(rippleFinished()),this,SLOT(handleRippleFinished()));
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
                    rippleCounter = 0;
                    rippleIntensityIncreasing = true;
                    click_pos = ev->localPos();
                    connect(t_update,SIGNAL(timeout()),this,SLOT(updateRippleData()));
                    connect(t_update,SIGNAL(timeout()),this,SLOT(repaint()));
                    t_update->start(10);
                    currentPaintFunction = std::bind(&MaterialButton::animateRipple,this,std::placeholders::_1);
                    QPushButton::mousePressEvent(ev);
                }
            private slots:
                void updateRippleData()
                {
                    const int rippleEnd = 10;
                    if (rippleCounter < rippleEnd && rippleIntensityIncreasing)
                        rippleCounter += 1;
                    else if (rippleCounter <= rippleEnd && !rippleIntensityIncreasing)
                        rippleCounter -= 1;
                    else if (rippleCounter == rippleEnd)
                        rippleIntensityIncreasing = false;
                    else if (rippleCounter == 0 && !rippleIntensityIncreasing)
                        emit rippleFinished();    
                }

                void handleRippleFinished()
                {
                    rippleCounter = 0;
                    rippleIntensityIncreasing = true;
                    click_pos = QPointF();
                    disconnect(t_update,SIGNAL(timeout()),this,SLOT(updateRippleData()));
                    t_update->stop();
                    currentPaintFunction = 0;
                }
            private:
                void animateRipple(QPainter* painter)
                {
                    QPen pen = painter->pen();
                    pen.setColor(Qt::white);
                    painter->setPen(pen);
/*                    
                    QRadialGradient radialGrad(QPointF(click_pos.x()/2, click_pos.y()/2), 100);
                    radialGrad.setColorAt(0,Qt::black);
                    radialGrad.setColorAt(1,Qt::white);
                    painter->setBrush(QBrush(radialGrad));
*/
                    painter->setBrush(QBrush(Qt::red));
                    painter->setOpacity(rippleCounter*0.1);
                    painter->drawEllipse(click_pos,50,50);
                }

                QTimer* t_update;
                bool rippleIntensityIncreasing;
                int rippleCounter;
                QPointF click_pos;
                std::function<void(QPainter*)> currentPaintFunction;
            signals:
                void rippleFinished();
        };
    }
}
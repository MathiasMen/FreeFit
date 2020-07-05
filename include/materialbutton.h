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
                    rippleOpacityCounter = 0;
                    rippleRadiusCounter = 0;
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
                    if (rippleIntensityIncreasing)
                        rippleOpacityCounter += 1;
                    else
                        rippleOpacityCounter -= 1;

                    rippleRadiusCounter += 1;

                    if (rippleOpacityCounter == 0 && !rippleIntensityIncreasing)
                        emit rippleFinished();

                    if (rippleOpacityCounter == rippleSteps)
                        rippleIntensityIncreasing = false;
                }

                void handleRippleFinished()
                {
                    rippleOpacityCounter = 0;
                    rippleRadiusCounter = 0;
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
                    painter->setBrush(QBrush(Qt::red));

                    painter->setOpacity(0.5*rippleOpacityCounter/rippleSteps);
                    painter->drawEllipse(click_pos,100*rippleRadiusCounter/rippleSteps,100*rippleRadiusCounter/rippleSteps);
                }

                QTimer* t_update;
                bool rippleIntensityIncreasing;
                const int rippleSteps = 10;
                int rippleOpacityCounter;
                int rippleRadiusCounter;
                QPointF click_pos;
                std::function<void(QPainter*)> currentPaintFunction;
            signals:
                void rippleFinished();
        };
    }
}
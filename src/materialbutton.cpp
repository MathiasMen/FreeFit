#include "include/materialbutton.h"

namespace FreeFit
{
    namespace GUI
   {
        MaterialButton::MaterialButton(QString t, QWidget* parent) : QPushButton(t,parent),t_update(new QTimer)
        {
            this->setStyleSheet("background-color:white; color:black; border: 2px solid red; padding: 20px;");
            this->setFlat(true);
            connect(this,SIGNAL(rippleFinished()),this,SLOT(handleRippleFinished()));
        }

        void MaterialButton::paintEvent(QPaintEvent* ev)
        {
            QPushButton::paintEvent(ev);
            QPainter painter(this);

            if(currentPaintFunction)
                currentPaintFunction(&painter);
        }

        void MaterialButton::mousePressEvent(QMouseEvent* ev)
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

        void MaterialButton::updateRippleData()
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

        void MaterialButton::handleRippleFinished()
        {
            rippleOpacityCounter = 0;
            rippleRadiusCounter = 0;
            rippleIntensityIncreasing = true;
            click_pos = QPointF();
            disconnect(t_update,SIGNAL(timeout()),this,SLOT(updateRippleData()));
            t_update->stop();
            currentPaintFunction = 0;
        }

        void MaterialButton::animateRipple(QPainter* painter)
        {
            QPen pen = painter->pen();
            pen.setColor(Qt::white);
            painter->setPen(pen);
            painter->setBrush(QBrush(Qt::red));

            painter->setOpacity(0.5*rippleOpacityCounter/rippleSteps);
            painter->drawEllipse(click_pos,100*rippleRadiusCounter/rippleSteps,100*rippleRadiusCounter/rippleSteps);
        }
   } 
}

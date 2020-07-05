#pragma once

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QPainter>
#include <QRect>
#include <QStyle>
#include <QStyleOption>
#include <QTimer>
#include <QStateMachine>
#include <QState>

#include <iostream>
#include <functional>

namespace FreeFit
{
    namespace GUI
    {
        class MaterialTextField : public QLineEdit
        {
        Q_OBJECT
        public:
            MaterialTextField(QString t_t, QWidget* parent = nullptr) : t_update(new QTimer),QLineEdit(t_t,parent),t(t_t),lineAnimationCounter(0),textAnimationCounter(0)
            {
                this->setStyleSheet("background-color:white; color:black; border: 2px; padding: 0px; padding-bottom: 2px; padding-top: 20px;");
                this->setAttribute(Qt::WA_MacShowFocusRect, 0);
                this->setFrame(false);

                connect(this,&MaterialTextField::focusGainedTextEntered,[=](){currentPaintFunction = std::bind(&MaterialTextField::focusGainedTextEnteredPaint,this,std::placeholders::_1,std::placeholders::_2); std::cout << "1" << std::endl;});
                connect(this,&MaterialTextField::focusGainedNoTextEntered,[=](){currentPaintFunction = std::bind(&MaterialTextField::focusGainedNoTextEnteredPaint,this,std::placeholders::_1,std::placeholders::_2); std::cout << "2" << std::endl;});
                connect(this,&MaterialTextField::focusLostTextEntered,[=](){currentPaintFunction = std::bind(&MaterialTextField::focusLostTextEnteredPaint,this,std::placeholders::_1,std::placeholders::_2); std::cout << "3" << std::endl;});
                connect(this,&MaterialTextField::focusLostNoTextEntered,[=](){currentPaintFunction = std::bind(&MaterialTextField::focusLostNoTextEnteredPaint,this,std::placeholders::_1,std::placeholders::_2); std::cout << "4" << std::endl;});
            }

        protected:
            void paintEvent(QPaintEvent* ev) override
            {
                QLineEdit::paintEvent(ev);

                QPainter painter(this);

                if (currentPaintFunction)
                    currentPaintFunction(&painter,this);

            }

            void focusInEvent(QFocusEvent* e) override
            {
                connect(t_update,SIGNAL(timeout()),this,SLOT(incrementLineAnimationCounter()));
                connect(t_update,SIGNAL(timeout()),this,SLOT(repaint()));
                t_update->start(20);

                if (text() != "")
                    emit focusGainedTextEntered();
                else
                    emit focusGainedNoTextEntered();

                QLineEdit::focusInEvent(e);
            }

            void focusOutEvent(QFocusEvent* e) override
            {
                t_update->stop();
                disconnect(t_update,SIGNAL(timeout()),this,SLOT(incrementLineAnimationCounter()));
                disconnect(t_update,SIGNAL(timeout()),this,SLOT(repaint()));
                lineAnimationCounter = 0;
                textAnimationCounter = 0;

                if (text() != "")
                    emit focusLostTextEntered();
                else
                    emit focusLostNoTextEntered();

                QLineEdit::focusOutEvent(e);
            }
        private:
            void setDefaultPainterSettings(QPainter* painter)
            {
                QPen pen = painter->pen();
                pen.setWidth(2);
                QFont font = painter->font();
                font.setPixelSize(10);

                painter->setPen(pen);
                painter->setFont(font);
            }
            
            void setFocusedPainterSettings(QPainter* painter)
            {
                setDefaultPainterSettings(painter);
                QPen pen = painter->pen();
                pen.setColor(Qt::red);
                painter->setPen(pen);
            }

            void setNotFocusedPainterSettings(QPainter* painter)
            {
                setDefaultPainterSettings(painter);
                QPen pen = painter->pen();
                pen.setColor(Qt::gray);
                painter->setPen(pen);
            }

            void drawAnimatedBaseLine(QPainter* painter, MaterialTextField* textfield)
            {
                painter->drawLine(textfield->rect().width()/2,textfield->rect().height()/2,textfield->rect().width()/2+5*lineAnimationCounter,textfield->rect().height()/2);
                if (textfield->rect().width()/2+5*lineAnimationCounter <= textfield->rect().width())
                {
                    painter->drawLine(textfield->rect().width()/2,textfield->rect().height()-1,textfield->rect().width()/2+5*lineAnimationCounter,textfield->rect().height()-1);
                    painter->drawLine(textfield->rect().width()/2,textfield->rect().height()-1,textfield->rect().width()/2-5*lineAnimationCounter,textfield->rect().height()-1);
                }
                else
                {
                    painter->drawLine(0,textfield->rect().height()-1,textfield->rect().width(),textfield->rect().height()-1);
                }
            }

            void focusGainedTextEnteredPaint(QPainter* painter, MaterialTextField* textfield)
            {
                setFocusedPainterSettings(painter);
                drawAnimatedBaseLine(painter,textfield);
            }

            void focusGainedNoTextEnteredPaint(QPainter* painter, MaterialTextField* textfield)
            {
                setFocusedPainterSettings(painter);
                drawAnimatedBaseLine(painter,textfield);
            }

            void focusLostTextEnteredPaint(QPainter* painter, MaterialTextField* textfield)
            {
                setNotFocusedPainterSettings(painter);
                painter->drawLine(0,textfield->rect().height(),textfield->rect().width(),textfield->rect().height());
                painter->drawText(0,painter->font().pixelSize(),t);
            }

            void focusLostNoTextEnteredPaint(QPainter* painter, MaterialTextField* textfield)
            {
                setNotFocusedPainterSettings(painter);
                painter->drawLine(0,textfield->rect().height(),textfield->rect().width(),textfield->rect().height());
            }

            int lineAnimationCounter;
            int textAnimationCounter;
            QTimer* t_update;
            QString t;

            std::function<void(QPainter*,FreeFit::GUI::MaterialTextField*)> currentPaintFunction;
        signals:
            void focusGainedTextEntered();
            void focusGainedNoTextEntered();
            void focusLostTextEntered();
            void focusLostNoTextEntered();
        private slots:
            void incrementLineAnimationCounter()
            {
                lineAnimationCounter += 1;
            }

            void incrementTextAnimationCounter()
            {
                textAnimationCounter += 1;
            }
        };
    }
}
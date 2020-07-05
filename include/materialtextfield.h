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
                this->setStyleSheet("background-color:white; color:black; border: 0px; padding: 0px; padding-bottom: 2px; padding-top: 20px;");
                this->setAttribute(Qt::WA_MacShowFocusRect, 0);
                this->setFrame(false);

                connect(this,&MaterialTextField::focusGainedTextEntered,[=](){currentPaintFunction = std::bind(&MaterialTextField::focusGainedTextEnteredPaint,this,std::placeholders::_1);});
                connect(this,&MaterialTextField::focusGainedNoTextEntered,[=](){currentPaintFunction = std::bind(&MaterialTextField::focusGainedNoTextEnteredPaint,this,std::placeholders::_1);});
                connect(this,&MaterialTextField::focusLostTextEntered,[=](){currentPaintFunction = std::bind(&MaterialTextField::focusLostTextEnteredPaint,this,std::placeholders::_1);});
                connect(this,&MaterialTextField::focusLostNoTextEntered,[=](){currentPaintFunction = std::bind(&MaterialTextField::focusLostNoTextEnteredPaint,this,std::placeholders::_1);});
            }

        protected:
            void paintEvent(QPaintEvent* ev) override
            {
                QLineEdit::paintEvent(ev);

                QPainter painter(this);

                if (currentPaintFunction)
                    currentPaintFunction(&painter);

            }

            void focusInEvent(QFocusEvent* e) override
            {
                connect(t_update,SIGNAL(timeout()),this,SLOT(incrementLineAnimationCounter()));
                connect(t_update,SIGNAL(timeout()),this,SLOT(incrementTextAnimationCounter()));
                connect(t_update,SIGNAL(timeout()),this,SLOT(repaint()));
                t_update->start(20);

                if (text() != "" && text() != t)
                    emit focusGainedTextEntered();
                else
                {
                    setText("");
                    emit focusGainedNoTextEntered();
                }

                QLineEdit::focusInEvent(e);
            }

            void focusOutEvent(QFocusEvent* e) override
            {
                t_update->stop();
                disconnect(t_update,SIGNAL(timeout()),this,SLOT(incrementLineAnimationCounter()));
                disconnect(t_update,SIGNAL(timeout()),this,SLOT(incrementTextAnimationCounter()));
                disconnect(t_update,SIGNAL(timeout()),this,SLOT(repaint()));
                lineAnimationCounter = 0;
                textAnimationCounter = 0;

                if (text() != "" && text() != t)
                    emit focusLostTextEntered();
                else
                {
                    setText(t);
                    emit focusLostNoTextEntered();
                }

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

            void drawStaticBaseLine(QPainter* painter)
            {
                painter->drawLine(0,this->rect().height(),this->rect().width(),this->rect().height());
            }

            void drawAnimatedBaseLine(QPainter* painter)
            {
                if (this->rect().width()/2+5*lineAnimationCounter <= this->rect().width())
                {
                    painter->drawLine(this->rect().width()/2,this->rect().height()-1,this->rect().width()/2+5*lineAnimationCounter,this->rect().height()-1);
                    painter->drawLine(this->rect().width()/2,this->rect().height()-1,this->rect().width()/2-5*lineAnimationCounter,this->rect().height()-1);
                }
                else
                {
                    painter->drawLine(0,this->rect().height(),this->rect().width(),this->rect().height());
                }
            }

            void drawStaticTextLabel(QPainter* painter)
            {
                painter->drawText(0,painter->font().pixelSize(),t);
            }

            void drawAnimatedTextLabel(QPainter* painter)
            {
                if (this->rect().height() - 2*textAnimationCounter >= painter->font().pixelSize())
                    painter->drawText(0,this->rect().height()-2*textAnimationCounter,this->t);
                else
                    drawStaticTextLabel(painter);
            }

            void focusGainedTextEnteredPaint(QPainter* painter)
            {
                setFocusedPainterSettings(painter);
                drawAnimatedBaseLine(painter);
                drawStaticTextLabel(painter);
            }

            void focusGainedNoTextEnteredPaint(QPainter* painter)
            {
                setFocusedPainterSettings(painter);
                drawAnimatedBaseLine(painter);
                drawAnimatedTextLabel(painter);
            }

            void focusLostTextEnteredPaint(QPainter* painter)
            {
                setNotFocusedPainterSettings(painter);
                drawStaticBaseLine(painter);
                drawStaticTextLabel(painter);
            }

            void focusLostNoTextEnteredPaint(QPainter* painter)
            {
                setNotFocusedPainterSettings(painter);
                drawStaticBaseLine(painter);
            }

            int lineAnimationCounter;
            int textAnimationCounter;
            QTimer* t_update;
            QString t;

            std::function<void(QPainter*)> currentPaintFunction;
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
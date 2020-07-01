#pragma once

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QPainter>
#include <QRect>
#include <QStyle>
#include <QStyleOption>
#include <QTimer>

#include <iostream>

namespace FreeFit
{
    namespace GUI
    {
        class MaterialTextField : public QLineEdit
        {
        Q_OBJECT
        public:
            MaterialTextField(QString t_t, QWidget* parent = nullptr) : t_update(new QTimer),QLineEdit(t_t,parent),t(t_t),focused(false),lineAnimationCounter(0),textAnimationCounter(0),lineAnimationFinished(false),textAnimationFinished(false)
            {
                this->setStyleSheet("background-color:white; color:black; border: 2px; padding: 0px; padding-top: 20px;");
                this->setAttribute(Qt::WA_MacShowFocusRect, 0);
                this->setFrame(false);
            }

        protected:
            void paintEvent(QPaintEvent* ev) override
            {
                QStyleOption opt;
                opt.init(this);
                QPainter p(this);
                style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

                QLineEdit::paintEvent(ev);
                
                QPainter painter(this);
                QPen pen = painter.pen();
                pen.setColor(Qt::gray);
                pen.setWidth(2);
                QFont font = painter.font();
                font.setPixelSize(10);

                painter.setPen(pen);
                painter.setFont(font);
                if (!focused)
                    painter.drawLine(0,this->rect().height(),this->rect().width(),this->rect().height());
                else
                {
                    painter.setPen(Qt::red);
                    
                    if (lineAnimationFinished)
                        painter.drawLine(0,this->rect().height()-1,this->rect().width(),this->rect().height()-1);
                    else
                    {
                        if (this->rect().width()/2+5*lineAnimationCounter <= this->rect().width())
                        {
                            painter.drawLine(this->rect().width()/2,this->rect().height()-1,this->rect().width()/2+5*lineAnimationCounter,this->rect().height()-1);
                            painter.drawLine(this->rect().width()/2,this->rect().height()-1,this->rect().width()/2-5*lineAnimationCounter,this->rect().height()-1);
                        }
                        else
                        {
                            lineAnimationFinished = true;
                            painter.drawLine(0,this->rect().height()-1,this->rect().width(),this->rect().height()-1);
                        }
                    }

                    if (textAnimationFinished)
                    {
                        painter.drawText(0,painter.font().pixelSize(),t);
                    }
                    else
                    {
                        if (this->rect().height() - 2*textAnimationCounter >= painter.font().pixelSize())
                        {
                            painter.drawText(0,this->rect().height()-2*textAnimationCounter,t);
                        }
                        else
                        {
                            textAnimationFinished = true;
                            painter.drawText(0,painter.font().pixelSize(),t);
                        }
                    }
                }
            }

            void focusInEvent(QFocusEvent* e) override
            {
                focused = true;
                if (text() == t)
                    setText("");
                connect(t_update,SIGNAL(timeout()),this,SLOT(updateAnimationData()));
                connect(t_update,SIGNAL(timeout()),this,SLOT(repaint()));
                t_update->start(20);
                QLineEdit::focusInEvent(e);
            }

            void focusOutEvent(QFocusEvent* e) override
            {
                focused = false;
                if (text() == "")
                    setText(t);
                t_update->stop();
                disconnect(t_update,SIGNAL(timeout()),this,SLOT(updateAnimationData()));
                disconnect(t_update,SIGNAL(timeout()),this,SLOT(repaint()));
                lineAnimationCounter = 0;
                lineAnimationFinished = false;
                textAnimationCounter = 0;
                textAnimationFinished = false;
                QLineEdit::focusOutEvent(e);
            }
        private:
            bool focused;
            bool lineAnimationFinished;
            bool textAnimationFinished;
            int lineAnimationCounter;
            int textAnimationCounter;
            QTimer* t_update;
            QString t;
        private slots:
            void updateAnimationData()
            {
                if (!lineAnimationFinished)
                    lineAnimationCounter += 1;

                if (!textAnimationFinished)
                    textAnimationCounter += 1;
            }
        };
    }
}
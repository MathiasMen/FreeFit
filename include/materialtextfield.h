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
            MaterialTextField(QString t, QWidget* parent = nullptr) : t_update(new QTimer),QLineEdit(t,parent),animationStarted(false),start(0),animationCounter(0)
            {
                this->setStyleSheet("background-color:white; color:black; border: 2px; padding: 0px; padding-top: 10px;");
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
                painter.setPen(pen);
                if (!animationStarted)
                    painter.drawLine(0,this->rect().height(),this->rect().width(),this->rect().height());
                else
                {
                    painter.setPen(Qt::red);
                    painter.drawLine(this->rect().width()/2,this->rect().height(),this->rect().width()/2+5*animationCounter,this->rect().height());
                    painter.drawLine(this->rect().width()/2,this->rect().height(),this->rect().width()/2-5*animationCounter,this->rect().height());
                }
            }

            void focusInEvent(QFocusEvent* e) override
            {
                t_update->start(20);
                animationStarted = true;
                connect(t_update,SIGNAL(timeout()),this,SLOT(updateAnimationData()));
                connect(t_update,SIGNAL(timeout()),this,SLOT(repaint()));
                QLineEdit::focusInEvent(e);
            }
        private:
            bool animationStarted;
            int start;
            int animationCounter;
            QTimer* t_update;
        private slots:
            void updateAnimationData()
            {
                start += 5;
                animationCounter += 1;
            }

            void endAnimation()
            {
                t_update->stop();
                disconnect(t_update,SIGNAL(timeout()),this,SLOT(endAnimation()));
            }
        };
    }
}
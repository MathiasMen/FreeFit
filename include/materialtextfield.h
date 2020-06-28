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
            MaterialTextField(QString t, QWidget* parent = nullptr) : QLineEdit(t,parent)
            {
                t_end = new QTimer;
                t_up = new QTimer;
                t_end->setSingleShot(true);
                this->setStyleSheet("background-color:white; color:black; border: 2px; padding-top: 10px;");
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
                painter.setPen(Qt::black);
                painter.drawLine(start,0,this->rect().width(),this->rect().height());
                painter.drawText(this->rect(),0,"Hallo");
            }

            void focusInEvent(QFocusEvent* e) override
            {
                t_end->start(1000);
                t_up->start(100);
                connect(t_up,&QTimer::timeout,this,&MaterialTextField::updateAnimationData);
                connect(t_up,SIGNAL(timeout()),this,SLOT(repaint()));
                connect(t_end,SIGNAL(timeout()),this,SLOT(endAnimation()));
                QLineEdit::focusInEvent(e);
            }
        private:
            int start = 0;
            QTimer* t_end;
            QTimer* t_up;
        private slots:
            void updateAnimationData()
            {
                start += 5;
            }

            void endAnimation()
            {
                t_end->stop();
                t_up->stop();
                disconnect(t_up,&QTimer::timeout,this,&MaterialTextField::updateAnimationData);
                disconnect(t_up,SIGNAL(timeout()),this,SLOT(repaint()));
                disconnect(t_end,SIGNAL(timeout()),this,SLOT(endAnimation()));
            }
        };
    }
}
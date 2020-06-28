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
        class MaterialButton : public QLineEdit
        {
        Q_OBJECT
        public:
            MaterialButton(QString t, QWidget* parent = nullptr) : QLineEdit(t,parent)
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
                
                t_end->start(5000);
                t_up->start(100);
                connect(t_up,&QTimer::timeout,this,&MaterialButton::updateAnimationData);
                connect(t_up,SIGNAL(timeout()),this,SLOT(repaint()));
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
        };
    }
}
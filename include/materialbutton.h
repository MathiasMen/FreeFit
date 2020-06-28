#pragma once

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QPainter>
#include <QRect>
#include <QStyle>
#include <QStyleOption>

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
                painter.drawLine(0,0,this->rect().width(),this->rect().height());
                painter.drawText(this->rect(),0,"Hallo");
            }
        };
    }
}
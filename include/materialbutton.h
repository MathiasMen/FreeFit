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
            MaterialButton(QString t, QWidget* parent = nullptr) : QLineEdit(parent)
            {
                this->setStyleSheet("border: 2px solid lightblue");
            }

        protected:
            void paintEvent(QPaintEvent* ev) override
            {
                QStyleOption opt;
                opt.init(this);
                QPainter p(this);
                style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

                QPainter painter(this);
                painter.setPen(Qt::red);
                painter.drawLine(0,0,this->rect().width(),this->rect().height());
            }
        };
    }
}
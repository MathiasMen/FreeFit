#pragma once

#include <QPushButton>
#include <QString>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QApplication>
#include <QStyle>
#include <QPainter>

#include <iostream>

namespace FreeFit
{
    namespace GUI
    {
        class ControlButton : public QPushButton
        {
        Q_OBJECT
        public:
            enum ButtonType { ForwardButton, BackwardButton};

            ControlButton(QString button_text, ButtonType t, QWidget* parent = nullptr) : QPushButton(button_text,parent), button_type(t)
            {
                this->setStyleSheet("background-color:white; color:red; text-align:left;");
                this->setMinimumSize(button_width,button_height);
                this->setMaximumSize(button_width,button_height);
                update();
            }
        protected:
            void paintEvent(QPaintEvent* ev) override
            {
                QPushButton::paintEvent(ev);
                QPainter painter(this);
                QPen pen = painter.pen();
                pen.setColor(Qt::red);
                painter.setPen(pen);
                const int pad = 4;
                if (button_type == ButtonType::ForwardButton)
                {
                    painter.drawLine(this->rect().width()-10-pad,this->rect().height()/2    , this->rect().width()-pad,this->rect().height()/2);
                    painter.drawLine(this->rect().width()-5 -pad,this->rect().height()/2 - 5, this->rect().width()-pad,this->rect().height()/2);
                    painter.drawLine(this->rect().width()-5 -pad,this->rect().height()/2 + 5, this->rect().width()-pad,this->rect().height()/2);                    
                }
                else
                {
                    painter.drawLine(pad,this->rect().height()/2, 10 + pad ,this->rect().height()/2    );
                    painter.drawLine(pad,this->rect().height()/2, 5  + pad ,this->rect().height()/2 - 5);
                    painter.drawLine(pad,this->rect().height()/2, 5  + pad ,this->rect().height()/2 + 5);
                }
            }
        private:
            ButtonType button_type;
            const int button_width = 140;
            const int button_height = 30;
        };
    }
}
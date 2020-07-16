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
            
            enum ButtonImportance {Primary, Secondary};

            ControlButton(QString button_text, ButtonType t, ButtonImportance i = ButtonImportance::Primary, QWidget* parent = nullptr) : QPushButton(button_text,parent), button_type(t), button_importance(i)
            {
                std::string css_string = "border-radius:6px; padding:4px; ";
                if (button_importance == ButtonImportance::Primary)
                    css_string += "background-color:red; color:white; ";
                else
                    css_string += "background-color:white; color:red; border-color:red;";
                if (button_type == ButtonType::ForwardButton)
                    css_string += "text-align:left;";
                else
                    css_string += "text-align:right;";
                this->setStyleSheet(QString::fromStdString(css_string));
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
            ButtonImportance button_importance;

            const int button_width = 140;
            const int button_height = 30;
        };
    }
}
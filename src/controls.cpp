#include "include/controls.h"

namespace FreeFit
{
    namespace GUI
    {

        void ControlButton::paintEvent(QPaintEvent* ev)
        {
            QPushButton::paintEvent(ev);
            QPainter painter(this);
            QPen pen = painter.pen();
            if (button_importance == ButtonImportance::Primary)
                pen.setColor(Qt::white);
            else
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

        ControlButton::ControlButton(QString button_text, ButtonType t, ButtonImportance i, QWidget* parent) : QPushButton(button_text,parent), button_type(t), button_importance(i)
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

    }
}
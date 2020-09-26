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
                pen.setColor(QColor(QString::fromStdString(color)));
            painter.setPen(pen);

            const int pad = 5;
            const int pixmap_height = 20;
            const int pixmap_width = 20;
            const int arrow_length = 15;
            const int tip_length = 5;
            if (button_type == ButtonType::ForwardButton)
            {
                const int y = (height() / 2);
                painter.drawLine(width() - pad - arrow_length, y, width() - pad, y);
                painter.drawLine(width() - pad, y, width() - pad - tip_length, y + tip_length);
                painter.drawLine(width() - pad, y, width() - pad - tip_length, y - tip_length);
            }
            else
            {
                const int y = (height() / 2);
                painter.drawLine(pad + arrow_length, y, pad, y);
                painter.drawLine(pad, y, pad + tip_length, y + tip_length);
                painter.drawLine(pad, y, pad + tip_length, y - tip_length);
            }
        }

        ControlButton::ControlButton(QString button_text, ButtonType t, ButtonImportance i, QWidget* parent)
            : QPushButton(button_text,parent), button_type(t), button_importance(i)
        {
            setColor("#ff0000");
        }

        void ControlButton::updateStyle()
        {
            css_string = "border-radius:6px; padding:4px; ";
            if (button_importance == ButtonImportance::Primary)
                css_string += "background-color:" + color + "; color:#ffffff; ";
            else
                css_string += "background-color:#ffffff; color:"+ color + "; border:2px solid " + color + "; ";
            if (button_type == ButtonType::ForwardButton)
                css_string += "text-align:left;";
            else
                css_string += "text-align:right;";
            this->setStyleSheet(QString::fromStdString(css_string));
            this->setMinimumSize(button_width,button_height);
            this->setMaximumSize(button_width,button_height);
            update();
        }

        void ControlButton::setColor(std::string c)
        {
            color = c;
            updateStyle();
        }
    }
}
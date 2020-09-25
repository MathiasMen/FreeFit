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

            const int pad = 5;
            const int pixmap_height = 20;
            const int pixmap_width = 20;
            if (button_type == ButtonType::ForwardButton)
            {
                if (!arrow.isNull())
                {
                    const int y = (height() - pixmap_height) / 2;
                    painter.drawPixmap(width() - pixmap_width - pad, y, arrow.pixmap(pixmap_width,pixmap_height));
                }
            }
            else
            {
                if (!arrow.isNull())
                {
                    const int y = (height() - pixmap_height) / 2;
                    painter.drawPixmap(pad, y, arrow.pixmap(pixmap_width,pixmap_height));
                }
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
            std::string icon_path = "${CMAKE_SOURCE_DIR}/tools/";

            if (button_importance == ButtonImportance::Primary && button_type == ButtonType::ForwardButton)
                icon_path += "arrow-right.svg";
            else if (button_importance == ButtonImportance::Primary && button_type == ButtonType::BackwardButton)
                icon_path += "arrow-left.svg";
            else if (button_importance == ButtonImportance::Secondary && button_type == ButtonType::ForwardButton)
                icon_path += "arrow-right-secondary.svg";
            else if (button_importance == ButtonImportance::Secondary && button_type == ButtonType::BackwardButton)
                icon_path += "arrow-left-secondary.svg";

            arrow = QIcon(QString::fromStdString(icon_path));

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
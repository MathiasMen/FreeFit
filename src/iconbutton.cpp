#include "include/iconbutton.h"

namespace FreeFit
{
    namespace GUI
    {
        void IconButton::paintEvent(QPaintEvent* ev)
        {
            const int pixmap_height = size;
            const int pixmap_width = size;
            const int padding = 2;

            QPushButton::paintEvent(ev);

            QPixmap p = symbol.pixmap(pixmap_width,pixmap_height);
            QPainter icon_painter(&p);
            icon_painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            icon_painter.setBrush(color);
            icon_painter.drawRect(p.rect());

            QIcon colored_icon = QIcon(p);

            QPainter painter(this);
            painter.setPen(color);
            painter.drawPixmap(padding,padding,pixmap_width,pixmap_height,colored_icon.pixmap(pixmap_width,pixmap_height));
            painter.drawEllipse(0,0,pixmap_width+2*padding,pixmap_height+2*padding);
        }
    }
}

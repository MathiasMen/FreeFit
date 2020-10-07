#include "include/iconbutton.h"

namespace FreeFit
{
    namespace GUI
    {
        void IconButton::paintEvent(QPaintEvent* ev)
        {
            const int pixmap_height = 20;
            const int pixmap_width = 20;

            QPushButton::paintEvent(ev);

            QPixmap p = symbol.pixmap(pixmap_width,pixmap_height);
            QPainter icon_painter(&p);
            icon_painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            icon_painter.setBrush(color);
            icon_painter.drawRect(p.rect());

            QIcon colored_icon = QIcon(p);

            QPainter painter(this);
            painter.drawPixmap(0,0,pixmap_width,pixmap_height,colored_icon.pixmap(pixmap_width,pixmap_height));
        }
    }
}

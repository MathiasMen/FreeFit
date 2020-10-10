#pragma once

#include <QWidget>
#include <QIcon>
#include <QPushButton>
#include <QPainter>
#include <QColor>

namespace FreeFit
{
    namespace GUI
    {
        class IconButton : public QPushButton
        {
        Q_OBJECT
        public:
            IconButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr, unsigned int t_size = 20, unsigned int t_padding = 2) : QPushButton(text,parent),symbol(icon),size(t_size),padding(t_padding),color(Qt::gray)
            {
                setFlat(true);
                this->setFixedSize(size+2*padding,size+2*padding);
            }

            void setIcon(QIcon icon)
            {
                symbol = icon;
                update();
            }

            void setColor(std::string c)
            {
                color = QColor(QString::fromStdString(c));
            }
        protected:
            void paintEvent(QPaintEvent* ev);
        private:
            const unsigned int size;
            const unsigned int padding;
            QIcon symbol;
            QColor color;
        };
    }
}
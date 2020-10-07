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
            IconButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr) : QPushButton(text,parent),symbol(icon),color(Qt::gray)
            {
                setFlat(true);
            }

            void setColor(std::string c)
            {
                color = QColor(QString::fromStdString(c));
            }
        protected:
            void paintEvent(QPaintEvent* ev);
        private:
            QIcon symbol;
            QColor color;
        };
    }
}
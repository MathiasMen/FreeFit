#pragma once

#include <QWidget>
#include <QIcon>
#include <QPushButton>
#include <QPainter>

namespace FreeFit
{
    namespace GUI
    {
        class IconButton : public QPushButton
        {
        Q_OBJECT
        public:
            IconButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr) : QPushButton(text,parent),symbol(icon){}

        protected:
            void paintEvent(QPaintEvent* ev);
        private:
            QIcon symbol;
        };
    }
}
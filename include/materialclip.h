#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QString>

namespace FreeFit
{
    namespace GUI
    {
        class MaterialClip : public QLabel
        {
        Q_OBJECT
        public:
            MaterialClip(QString t = "", QWidget* parent = nullptr);
            void select();
            void deselect();
            void setInitials(QString s);
        protected:
            void mousePressEvent(QMouseEvent* ev) override;
        signals:
            void clicked(MaterialClip*);
        private:
            void updateStyle();

            bool selected = false;
            QString css_string = "background-color:red; color:black; border-radius:25px; text-align:center;";
        };
    }
}
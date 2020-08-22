#pragma once 

#include <QWidget>
#include <QLabel>
#include <QString>

namespace FreeFit
{
    namespace GUI
    {
        class ToggleableLabel : public QLabel
        {
            Q_OBJECT
        public:
            ToggleableLabel(QString text, QWidget* parent);
            void setSelectable(bool s);
            bool isToggled(){return toggled;}
            void click(){clicked_impl();};
        signals:
            void clicked();
        protected:
            void mousePressEvent(QMouseEvent* ev) override;
        private slots:
            void clicked_impl();
        private:
            bool toggled = false;
            QString css_string;
        };
    }
}
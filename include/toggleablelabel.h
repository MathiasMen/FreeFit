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
            void setColor(std::string c);
            bool isToggled(){return toggled;}
            void click(){clicked_impl();};
        signals:
            void clicked();
        protected:
            void mousePressEvent(QMouseEvent* ev) override;
        private slots:
            void clicked_impl();
        private:
            void updateCSS();

            bool toggled;

            std::string color;
        };
    }
}
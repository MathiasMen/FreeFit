#pragma once 

namespace FreeFit
{
    namespace GUI
    {
        class ToggleableLabel : public QLabel
        {
            Q_OBJECT
        friend ExerciseEditorValidator;
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
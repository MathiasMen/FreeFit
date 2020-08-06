#pragma once

#include <QPushButton>
#include <QString>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QApplication>
#include <QStyle>
#include <QPainter>

namespace FreeFit
{
    namespace GUI
    {
        class ControlButton : public QPushButton
        {
        Q_OBJECT
        public:
            enum ButtonType { ForwardButton, BackwardButton};
            
            enum ButtonImportance {Primary, Secondary};

            ControlButton(QString button_text, ButtonType t, ButtonImportance i = ButtonImportance::Primary, QWidget* parent = nullptr);
        protected:
            void paintEvent(QPaintEvent* ev) override;
        private:
            ButtonType button_type;
            ButtonImportance button_importance;

            const int button_width = 140;
            const int button_height = 30;
        };
    }
}
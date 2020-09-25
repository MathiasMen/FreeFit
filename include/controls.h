#pragma once

#include <QPushButton>
#include <QString>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QApplication>
#include <QStyle>
#include <QPainter>
#include <QIcon>
#include <QPixmap>

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

            void setColor(std::string c);

            void updateStyle();
        protected:
            void paintEvent(QPaintEvent* ev) override;
        private:
            ButtonType button_type;
            ButtonImportance button_importance;

            const int button_width = 140;
            const int button_height = 30;

            QIcon arrow;

            std::string color;
            std::string css_string;
        };
    }
}
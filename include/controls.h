#pragma once

#include <QPushButton>
#include <QString>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QApplication>
#include <QStyle>

namespace FreeFit
{
    namespace GUI
    {
        class ControlButton : public QPushButton
        {
        Q_OBJECT
        public:
            enum ButtonType { ForwardButton, BackwardButton};

            ControlButton(QString button_text, ButtonType t, QWidget* parent = nullptr) : QPushButton(parent)
            {
                this->setLayout(new QGridLayout);
                l = new QLabel(button_text,this);

                if (t == ButtonType::ForwardButton)
                {
                    this->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowForward));
                    this->setStyleSheet("text-align:right;");
                    l->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                }
                else
                {
                    this->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowBack));
                    this->setStyleSheet("text-align:left;");
                    l->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                }

                l->setAttribute(Qt::WA_TransparentForMouseEvents, true);
                this->layout()->addWidget(l);
                this->setIconSize(l->size());
            }
        private:
            QLabel* l;
        };
    }
}
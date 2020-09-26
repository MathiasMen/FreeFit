#pragma once

#include <QDialog>
#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

#include <string>

#include "include/controls.h"

namespace FreeFit
{
    namespace GUI
    {
        class MaterialDialog : public QDialog
        {
        public:
            MaterialDialog( QString reject_button_str = "",
                            QString accept_button_str = "",
                            QString skip_button_str = "",
                            QWidget* parent = nullptr,
                            Qt::WindowFlags f = Qt::WindowFlags())
                : QDialog(parent,f),reject_button(nullptr),skip_button(nullptr),accept_button(nullptr)
            {
                ly = new QVBoxLayout(this);
                content_layout = new QGridLayout();
                control_layout = new QHBoxLayout();

                if (!reject_button_str.isEmpty())
                    reject_button = new ControlButton(reject_button_str,ControlButton::BackwardButton,ControlButton::Primary,this);
                if (!accept_button_str.isEmpty())
                    accept_button = new ControlButton(accept_button_str,ControlButton::ForwardButton,ControlButton::Primary,this);
                if (!skip_button_str.isEmpty())
                    skip_button = new ControlButton(skip_button_str,ControlButton::ForwardButton,ControlButton::Secondary,this);

                QSpacerItem* horizontal_spacer_control = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);

                if (reject_button != nullptr)
                    control_layout->addWidget(reject_button,0,Qt::AlignLeft);
                
                control_layout->addItem(horizontal_spacer_control);

                if (skip_button != nullptr)
                    control_layout->addWidget(skip_button,0,Qt::AlignLeft);
                if (accept_button != nullptr)
                    control_layout->addWidget(accept_button,0,Qt::AlignLeft);

                QSpacerItem* vertical_spacer = new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
                ly->addLayout(content_layout);
                ly->addItem(vertical_spacer);
                ly->addLayout(control_layout);

                setColor("#ff0000");
            }

            ControlButton* getRejectButton(){return reject_button;}
            ControlButton* getAcceptButton(){return accept_button;}
            ControlButton* getSkipButton(){return skip_button;}

            void addWidget(QWidget* w,int row, int col,int row_span = 1, int col_span = 1, Qt::Alignment al = Qt::Alignment()){content_layout->addWidget(w,row,col,row_span,col_span,al);}
            void addItem(QLayoutItem* w,int row, int col,int row_span = 1, int col_span = 1, Qt::Alignment al = Qt::Alignment()){content_layout->addItem(w,row,col,row_span,col_span,al);}
            void addLayout(QLayout* l,int row, int col,int row_span = 1, int col_span = 1, Qt::Alignment al = Qt::Alignment()){content_layout->addLayout(l,row,col,row_span,col_span,al);}

            void setColor(std::string c)
            {
                color = c;
                updateStyle();
                setColorOfButton(c,reject_button);
                setColorOfButton(c,accept_button);
                setColorOfButton(c,skip_button);
            }

            void updateStyle()
            {
                setStyleSheet(QString::fromStdString("background-color:#ffffff; color:" + color + ";"));
            }
        private:
            void setColorOfButton(std::string c, ControlButton* b)
            {
                if (b != nullptr)
                    b->setColor(c);
            }

            QVBoxLayout* ly;
            QGridLayout* content_layout;
            QHBoxLayout* control_layout;

            ControlButton* reject_button;
            ControlButton* accept_button;
            ControlButton* skip_button;

            std::string color;     
        };
    }
}
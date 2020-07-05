#pragma once

#include <QPushButton>

#include <iostream>

namespace FreeFit
{
    namespace GUI
    {
        class MaterialButton : public QPushButton
        {
            public:
                MaterialButton(QString t, QWidget* parent = nullptr) : QPushButton(t,parent)
                {
                    this->setStyleSheet("background-color:white; color:black; border: 0px; padding: 0px;");
                    this->setFlat(true);
                }
        };
    }
}
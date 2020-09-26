#pragma once

#include <QDialog>
#include <QWidget>

#include <string>

namespace FreeFit
{
    namespace GUI
    {
        class MaterialDialog : public QDialog
        {
        public:
            MaterialDialog(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()) : QDialog(parent,f)
            {
                setColor("#ff0000");
            }

            void setColor(std::string c)
            {
                color = c;
                updateStyle();
            }

            void updateStyle()
            {
                setStyleSheet(QString::fromStdString("background-color:#ffffff; color:" + color + ";"));
            }
        private:
            std::string color;            
        };
    }
}
#pragma once

#include <QDialog>
#include <QWidget>

namespace FreeFit
{
    namespace GUI
    {
        class MaterialDialog : public QDialog
        {
        public:
            MaterialDialog(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()) : QDialog(parent,f)
            {
                setStyleSheet("background-color:white; color:black;");
            }
        };
    }
}
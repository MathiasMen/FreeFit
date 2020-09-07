#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

namespace FreeFit
{
    namespace GUI
    {
        class ProfileItem : public QWidget
        {
        public:
            ProfileItem(QString t_name = "new", QWidget* t_parent = nullptr) : QWidget(t_parent)
            {
                const int width = 50;
                const int height = 50;
                this->setFixedSize(width,height);
                ly = new QGridLayout(this);
                name_label = new QLabel(this);
                name_label->setFixedSize(width,height);
                setName(t_name);
            }
            void setName(QString name)
            {
                name_label->setText(name);
            }
        private:
            QGridLayout* ly;
            QLabel* name_label;
        };
    }
}
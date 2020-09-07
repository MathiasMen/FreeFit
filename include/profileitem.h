#pragma once

#include <QWidget>

namespace FreeFit
{
    namespace GUI
    {
        class ProfileItem : public QWidget
        {
        public:
            ProfileItem(QString t_name = "new", QWidget* t_parent = nullptr) : QWidget(t_parent),name(t_name.toStdString())
            {

            }
        private:
            std::string name;
        };
    }
}
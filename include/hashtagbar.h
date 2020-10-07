#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QSpacerItem>

#include "include/toggleablelabel.h"

namespace FreeFit
{
    namespace GUI
    {
        class HashtagBar : public QWidget
        {
            public:
                HashtagBar(QWidget* parent = nullptr);
                
                void setColor(std::string c);

                void addHashtag(std::string tag_str);

                std::list<std::string> getToggledStrings();

                void toggleHashtag(std::string tag_str);

                bool isHashtagToggled(std::string tag_str);

                void setCategoryName(std::string c){category_name = c;}

                std::string getCategoryName(){return category_name;}

                void setLabelsSelectable(bool s);
            private:
                QHBoxLayout* ly;
                std::vector<ToggleableLabel*> hashtag_labels;

                std::string category_name;
        };
    }
}
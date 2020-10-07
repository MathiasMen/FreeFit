#include "include/hashtagbar.h"

namespace FreeFit
{
    namespace GUI
    {
        HashtagBar::HashtagBar(QWidget* parent) : QWidget(parent)
        {
            ly = new QHBoxLayout(this);
            QSpacerItem* horizontal_spacer = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
            ly->addItem(horizontal_spacer);
        }
        
        void HashtagBar::setColor(std::string c)
        {
            for (auto l : hashtag_labels)
                l->setColor(c);
        }

        void HashtagBar::addHashtag(std::string tag_str)
        {
            ToggleableLabel* t = new ToggleableLabel("#" + QString::fromStdString(tag_str),this);
            ly->insertWidget(0,t);
            hashtag_labels.push_back(t);
        }

        std::list<std::string> HashtagBar::getToggledStrings()
        {
            std::list<std::string> l;
            for (auto label : hashtag_labels)
                if(label->isToggled())
                    l.push_back(label->text().remove(QChar('#')).toStdString());
            return l;
        }

        void HashtagBar::toggleHashtag(std::string tag_str)
        {
            for (auto h : hashtag_labels)
                if (h->text().remove(QChar('#')).toStdString() == tag_str)
                    h->click();
        }

        bool HashtagBar::isHashtagToggled(std::string tag_str)
        {
            for (auto h : hashtag_labels)
                if (h->text().remove(QChar('#')).toStdString() == tag_str)
                    return h->isToggled();
            return false;
        }

        void HashtagBar::setLabelsSelectable(bool s)
        {
            for (auto l : hashtag_labels)
                l->setSelectable(s);
        }
    
    }
}
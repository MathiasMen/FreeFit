#pragma once

#include <QWidget>
#include <QHBoxLayout>

#include "include/toggleablelabel.h"

namespace FreeFit
{
    namespace GUI
    {
        class HashtagBar : public QWidget
        {
            public:
                HashtagBar(QWidget* parent = nullptr) : QWidget(parent)
                {
                    ly = new QHBoxLayout(this);
                }

                void addHashtag(std::string tag_str)
                {
                    ToggleableLabel* t = new ToggleableLabel("#" + QString::fromStdString(tag_str),this);
                    ly->addWidget(t,0,Qt::AlignLeft);
                    hashtag_labels.push_back(t);
                }

                std::list<std::string> getToggledStrings()
                {
                    std::list<std::string> l;
                    for (auto label : hashtag_labels)
                        if(label->isToggled())
                            l.push_back(label->text().remove(QChar('#')).toStdString());
                    return l;
                }

                void toggleHashtag(std::string tag_str)
                {
                    for (auto h : hashtag_labels)
                        if (h->text().remove(QChar('#')).toStdString() == tag_str)
                            h->click();
                }

            private:
                QHBoxLayout* ly;
                std::vector<ToggleableLabel*> hashtag_labels;
        };
    }
}
#pragma once

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QPainter>
#include <QPen>

#include "include/materialtextfield.h"

namespace FreeFit
{
    namespace GUI
    {
        class ProfileItemValidator;
        class ProfileItemGroup;
        
        struct ProfileEditPopupResult
        {
            ProfileEditPopupResult(QString n, bool v)
                : name(n),name_valid(v)
            {

            }
            QString name;
            bool name_valid;
        };

        class ProfileEditPopup : public QWidget
        {
        Q_OBJECT
        public: 
            ProfileEditPopup(QString n, QWidget* parent = nullptr) : QWidget(parent, Qt::Popup)
            {
                ly = new QGridLayout(this);
                name = new MaterialTextField("Name",this);
                std::regex name_regex("[a-zA-Z\\s]{1,128}");
                auto func_name_regex = [name_regex](std::string s)->bool{return std::regex_match(s,name_regex);};
                name->setValidationFunction(func_name_regex);

                name->setText(n);

                ly->addWidget(name);
            }
        
            bool validateName()
            {
                return name->validateText();
            }

            QString getName()
            {
                return name->text();
            }
        signals:
            void popupFinished(ProfileEditPopupResult);
        protected:
            void closeEvent(QCloseEvent* e)
            {
                QWidget::closeEvent(e);
                emit popupFinished(ProfileEditPopupResult(name->text(),name->validateText()));
            }
        private:
            QGridLayout* ly;
            MaterialTextField* name;
        };

        class ProfileEditButton : public QPushButton
        {
        Q_OBJECT
        public:
            ProfileEditButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr) : QPushButton(text,parent)
            {
                symbol = QIcon(icon);
            }

        protected:
            void paintEvent(QPaintEvent* ev)
            {
                const int pixmap_height = 20;
                const int pixmap_width = 20;

                QPushButton::paintEvent(ev);
                
                QPixmap p = symbol.pixmap(pixmap_width,pixmap_height);
                QPainter icon_painter(&p);
                icon_painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                icon_painter.setBrush(Qt::gray);
                icon_painter.drawRect(p.rect());

                QIcon colored_icon = QIcon(p);

                QPainter painter(this);
                painter.drawPixmap(0,0,pixmap_width,pixmap_height,colored_icon.pixmap(pixmap_width,pixmap_height));
            }
        private:
            QIcon symbol;
        };

        class ProfileItem : public QWidget
        {
        friend ProfileItemValidator;
        Q_OBJECT
        public:
            ProfileItem(QString t_name = "new", QWidget* t_parent = nullptr) : QWidget(t_parent)
            {
                const int width = 200;
                const int height = 200;
                this->setFixedSize(width,height);

                ly = new QGridLayout(this);
                
                name_label = new QLabel(this);
                name_label->setFixedSize(width*0.9,height*0.9);
                name_label->setAlignment(Qt::AlignCenter);
                
                edit_button = new ProfileEditButton(QIcon("/Users/mathias/Documents/programming_workspace/FreeFit/tools/edit.svg"),"",this);
                edit_button->setFixedSize(width*0.1,height*0.1);
                edit_button->setStyleSheet("color:grey; border-style:none;");
                connect(edit_button,&QPushButton::clicked,this,[this]()
                {
                    ProfileEditPopup* p = new ProfileEditPopup(name_label->text(),this->edit_button);
                    p->move(QWidget::mapToGlobal(edit_button->pos() + edit_button->rect().bottomRight()));
                    p->show();
                    connect(p,SIGNAL(popupFinished(ProfileEditPopupResult)),this,SLOT(handlePopupFinished(ProfileEditPopupResult)));
                });
                setName(t_name);
                updateStyle();

                ly->addWidget(name_label,0,0,Qt::AlignCenter);
                ly->addWidget(edit_button,1,0,Qt::AlignCenter);
            }

            std::string getName()
            {
                return name_label->text().toStdString();
            }

            void setName(QString name)
            {
                for (unsigned int i = 0; i < 5; i++)
                    decreaseLabelTextSizeIfNecessary(name);
                name = elidedTextIfNecessary(name);
                name_label->setText(name);
                emit nameChanged(name.toStdString());
            }

            bool getSelected()
            {
                return selected;
            }

            void setSelected(bool b)
            {
                selected = b;
                updateStyle();
            }

            void setGroupPointer(ProfileItemGroup* p)
            {
                group_ptr = p;
            }
        public slots:
            void handlePopupFinished(ProfileEditPopupResult p)
            {
                if (p.name_valid)
                    setName(p.name);
            }
        signals:
            void nameChanged(std::string);
        protected:
            void mousePressEvent(QMouseEvent* ev)
            {
                QWidget::mousePressEvent(ev);
                if (group_ptr == nullptr)
                {
                    selected = !selected;
                    updateStyle();
                }
                else
                    emit itemPressed(this);
            }

        private:
            void updateStyle()
            {
                if (selected)
                    css_string.replace("color:grey; border: 2px solid grey;","color:red; border: 2px solid red;");
                else
                    css_string.replace("color:red; border: 2px solid red;","color:grey; border: 2px solid grey;");
                setStyleSheet(css_string);
            }

            void decreaseLabelTextSizeIfNecessary(QString s)
            {
                QFont font = name_label->font();
                QRect bounds = name_label->rect();
                QRect font_bounds = QFontMetrics(font).boundingRect(s);
                if (font_bounds.width() > bounds.width() - 2 || font_bounds.height() > bounds.height() - 2)
                    font.setPointSize(font.pointSize() - 1);
                name_label->setFont(font);
            }

            QString elidedTextIfNecessary(QString s)
            {
                QFont font = name_label->font();
                QRect bounds = name_label->rect();
                QRect font_bounds = QFontMetrics(font).boundingRect(s);

                if (font_bounds.width() > bounds.width() -2 || font_bounds.height() > bounds.height() - 2)
                    return QFontMetrics(font).elidedText(s,Qt::ElideRight,bounds.width() - 2);
                else
                    return s;
            }

            QGridLayout* ly;
            QLabel* name_label;
            ProfileEditButton* edit_button;

            bool selected = false;
            ProfileItemGroup* group_ptr = nullptr;
            QString css_string = "color:grey; border: 2px solid grey; border-radius:5px; text-align:center;";
        signals:
            void itemPressed(ProfileItem* i);
        };

        class ProfileItemGroup : public QObject
        {
        friend ProfileItemValidator;
        Q_OBJECT
        public:
            ProfileItemGroup(QObject* parent = nullptr) : QObject(parent),current_profile(nullptr)
            {

            }

            void addItem(ProfileItem* i)
            {
                i->setGroupPointer(this);
                items.push_back(i);
                connect(i,SIGNAL(itemPressed(ProfileItem*)),this,SLOT(itemPressed(ProfileItem*)));
                connect(i,SIGNAL(nameChanged(std::string)),this,SIGNAL(currentNameChanged(std::string)));
            }

            int currentIndex()
            {
                int i = -1;
                int c = 0;
                for (auto pi : items)
                {
                    if (pi == current_profile)
                        i = c;
                    c += 1;
                }
                return i;
            }

            std::string getCurrentName()
            {
                if (current_profile != nullptr)
                    return current_profile->getName();
                else
                    return "";
            }

            void setCurrentName(QString n)
            {
                if (current_profile != nullptr)
                    current_profile->setName(n);
            }

            void selectProfile(int i)
            {
                itemPressed(items[i]);
            }

            std::vector<ProfileItem*> getItems(){return items;}
        public slots:
            void itemPressed(ProfileItem* i)
            {
                for (auto item : items)
                    item->setSelected(false);
                i->setSelected(true);
                current_profile = i;
            }
        signals:
            void currentNameChanged(std::string);
        private:
            std::vector<ProfileItem*> items;
            ProfileItem* current_profile;
        };

        class ProfileItemValidator
        {
        public:
            ProfileItemValidator(ProfileItemGroup* t_g) : g(t_g){}

            void selectProfileItem(int i)
            {
                g->selectProfile(i);
            }

            std::string getProfileCSSString(int i)
            {
                return g->items[i]->styleSheet().toStdString();
            }

            std::string getName(int i)
            {
                return g->items[i]->getName();
            }

            std::string getCurrentName()
            {
                return g->current_profile->getName();
            }
        private:
            ProfileItemGroup* g;
        };
    }
}
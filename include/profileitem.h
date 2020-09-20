#pragma once

#include <vector>

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QIcon>
#include <QPixmap>
#include <QPushButton>
#include <QPainter>
#include <QPen>
#include <QSpacerItem>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRegExp>

#include "include/materialtextfield.h"


namespace FreeFit
{
    namespace GUI
    {
        class ProfileItemValidator;
        class ProfileItemGroup;

        class ProfileEditColorPickerTile : public QPushButton
        {
        Q_OBJECT
        public:
            ProfileEditColorPickerTile(QColor c, QWidget* parent = nullptr) : QPushButton(nullptr),color(c)
            {
                QPixmap p(20,20);
                p.fill(color);
                this->setIcon(p);
                this->setCheckable(true);
            }

            QString getColor()
            {
                return color.name();
            }
        public slots:
            void updateCSS()
            {
                if (this->isChecked())
                    this->setStyleSheet("border: 4px solid white;");
                else
                    this->setStyleSheet("");
            }
        private:
            QColor color;
        };

        class ProfileEditColorPicker : public QGroupBox
        {
        public:
            ProfileEditColorPicker(QWidget* parent = nullptr) : QGroupBox("Color",parent)
            {
                ly = new QGridLayout(this);
                grp = new QButtonGroup(this);
                grp->setExclusive(true);

                unsigned int row_counter = 0;
                unsigned int col_counter = 0;
                for (auto c : colors)
                {
                    ProfileEditColorPickerTile* t = new ProfileEditColorPickerTile(c,this);

                    ly->addWidget(t,row_counter,col_counter);
                    grp->addButton(t);
                    color_tiles.push_back(t);
                    connect(grp,SIGNAL(buttonClicked(QAbstractButton*)),t,SLOT(updateCSS()));

                    if (row_counter == 0)
                        row_counter = 1;
                    else
                    {
                        row_counter = 0;
                        col_counter += 1;
                    }                    
                }
                this->setLayout(ly);
            }

            void selectColor(QColor c)
            {
                
            }

            QString getColorName()
            {
                for (auto t : color_tiles)
                    if (t->isChecked())
                        return t->getColor();
                return QString();
            }
        private:
            std::vector<QColor> colors = {Qt::red,Qt::blue,Qt::green,Qt::yellow};
            std::vector<ProfileEditColorPickerTile*> color_tiles;
            QGridLayout* ly;
            QButtonGroup* grp;
        };

        struct ProfileEditPopupResult
        {
            ProfileEditPopupResult(QString n, QString c, bool v)
                : name(n),color_name(c),name_valid(v)
            {

            }
            QString name;
            QString color_name;
            bool name_valid;
        };

        class ProfileEditPopup : public QWidget
        {
        Q_OBJECT
        public: 
            ProfileEditPopup(QString n, QWidget* parent = nullptr);
        
            bool validateName(){return name->validateText();}

            QString getName(){return name->text();}

            QString getColorName(){return p->getColorName();}
        signals:
            void popupFinished(ProfileEditPopupResult);
        protected:
            void closeEvent(QCloseEvent* e);
        private:
            QGridLayout* ly;
            MaterialTextField* name;
            ProfileEditColorPicker* p;
            QSpacerItem* horizontal_spacer;
        };

        class ProfileEditButton : public QPushButton
        {
        Q_OBJECT
        public:
            ProfileEditButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr) : QPushButton(text,parent),symbol(icon){}

        protected:
            void paintEvent(QPaintEvent* ev);
        private:
            QIcon symbol;
        };

        class ProfileItem : public QWidget
        {
        friend ProfileItemValidator;
        Q_OBJECT
        public:
            ProfileItem(QString t_name = "new", QWidget* t_parent = nullptr);

            std::string getName(){return name_label->text().toStdString();}

            void setName(QString name);

            bool getSelected(){return selected;}

            void setSelected(bool b);

            void setGroupPointer(ProfileItemGroup* p){group_ptr = p;}

            void setColor(QString color)
            {
                setColorInCSS(color);
                this->setStyleSheet(css_string);
            };
        public slots:
            void handlePopupFinished(ProfileEditPopupResult p);
        signals:
            void nameChanged(std::string);
        protected:
            void mousePressEvent(QMouseEvent* ev);
        private:
            void setColorInCSS(QString c)
            {
                css_string.replace(QRegExp("#([0-9]|[a-f]|[A-F]){6}"),c);
            }

            void updateStyle();

            void decreaseLabelTextSizeIfNecessary(QString s);

            QString elidedTextIfNecessary(QString s);

            QGridLayout* ly;
            QLabel* name_label;
            ProfileEditButton* edit_button;

            bool selected = false;
            ProfileItemGroup* group_ptr = nullptr;
            QString css_string = "color:#808080; border: 2px solid #808080; border-radius:5px; text-align:center;";
        signals:
            void itemPressed(ProfileItem* i);
        };

        class ProfileItemGroup : public QObject
        {
        friend ProfileItemValidator;
        Q_OBJECT
        public:
            ProfileItemGroup(QObject* parent = nullptr) : QObject(parent),current_profile(nullptr){}

            void addItem(ProfileItem* i);

            int currentIndex();

            std::string getCurrentName();

            void setCurrentName(QString n);

            void selectProfile(int i){itemPressed(items[i]);}

            std::vector<ProfileItem*> getItems(){return items;}
        public slots:
            void itemPressed(ProfileItem* i);
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

            void selectProfileItem(int i){g->selectProfile(i);}

            std::string getProfileCSSString(int i){return g->items[i]->styleSheet().toStdString();}

            std::string getName(int i){return g->items[i]->getName();}

            std::string getCurrentName(){return g->current_profile->getName();}
        private:
            ProfileItemGroup* g;
        };
    }
}
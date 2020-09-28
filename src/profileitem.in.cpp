#include "include/profileitem.h"

#include <iostream>

namespace FreeFit
{
    namespace GUI
    {
        ProfileEditColorPickerTile::ProfileEditColorPickerTile(QColor c, QWidget* parent) : QPushButton(nullptr),color(c)
        {
            this->setContentsMargins(0,0,0,0);
            this->setFixedSize(20,20);
            this->setCheckable(true);
            updateCSS();
            QPixmap p(20,20);
            p.fill(color);
            this->setIcon(p);
        }

        void ProfileEditColorPickerTile::updateCSS()
        {
            if (this->isChecked())
                this->setStyleSheet("border: 2px solid white; border-radius: 0px; padding: 0px 0px 0px 0px;");
            else
                this->setStyleSheet("border: 2px solid grey; border-radius: 0px; padding: 0px 0px 0px 0px;");
        }

        ProfileEditColorPicker::ProfileEditColorPicker(QWidget* parent) : QWidget(parent)
        {
            this->setContentsMargins(0,0,0,0);
            ly = new QGridLayout(this);
            ly->setContentsMargins(0,0,0,0);
            color_ly = new QGridLayout;
            color_ly->setContentsMargins(0,0,0,0);

            color_lbl = new QLabel("Color",this);
            ly->addWidget(color_lbl,0,0,Qt::AlignLeft);

            grp = new QButtonGroup(this);
            grp->setExclusive(true);

            unsigned int row_counter = 0;
            unsigned int col_counter = 0;
            for (auto c : colors)
            {
                ProfileEditColorPickerTile* t = new ProfileEditColorPickerTile(c,this);

                color_ly->addWidget(t,row_counter,col_counter,Qt::AlignLeft);
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

            ly->addLayout(color_ly,1,0,Qt::AlignLeft);
            this->setLayout(ly);
        }

        QString ProfileEditColorPicker::getColorName()
        {
            for (auto t : color_tiles)
                if (t->isChecked())
                    return t->getColor();
            return QString();
        }

        ProfileEditPopup::ProfileEditPopup(QString n, QWidget* parent) : QWidget(parent, Qt::Popup)
        {
            ly = new QGridLayout(this);
            name = new MaterialTextField("Name",this);
            p = new ProfileEditColorPicker(this);
            std::regex name_regex("[a-zA-Z\\s]{1,128}");
            auto func_name_regex = [name_regex](std::string s)->bool{return std::regex_match(s,name_regex);};
            name->setValidationFunction(func_name_regex);

            name->setText(n);

            horizontal_spacer = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);

            ly->addWidget(name,0,0,1,2);
            ly->addWidget(p,1,0,1,1);
            ly->addItem(horizontal_spacer,1,1,1,1);
        }

        void ProfileEditPopup::closeEvent(QCloseEvent* e)
        {
            QWidget::closeEvent(e);
            emit popupFinished(ProfileEditPopupResult(name->text(),p->getColorName(),name->validateText()));
        }

        void ProfileEditButton::paintEvent(QPaintEvent* ev)
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

        ProfileItem::ProfileItem(QString t_name, QColor t_color, QWidget* t_parent) : QWidget(t_parent),item_color(t_color)
        {
            const int width = 200;
            const int height = 200;
            this->setFixedSize(width,height);

            ly = new QGridLayout(this);
            
            name_label = new QLabel(this);
            name_label->setFixedSize(width*0.9,height*0.9);
            name_label->setAlignment(Qt::AlignCenter);
            
            edit_button = new ProfileEditButton(QIcon("${CMAKE_SOURCE_DIR}/tools/edit.svg"),"",this);
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
            setColor(t_color.name());
            updateStyle();

            ly->addWidget(name_label,0,0,Qt::AlignCenter);
            ly->addWidget(edit_button,1,0,Qt::AlignCenter);
        }

        void ProfileItem::setName(QString name)
        {
            for (unsigned int i = 0; i < 5; i++)
                decreaseLabelTextSizeIfNecessary(name);
            name = elidedTextIfNecessary(name);
            name_label->setText(name);
            emit nameChanged(name.toStdString());
        }

        void ProfileItem::setSelected(bool b)
        {
            selected = b;
            updateStyle();
        }

        void ProfileItem::setColor(QString color)
        {
            item_color = QColor(color);
            setColorInCSS(color);
            this->setStyleSheet(css_string);
            emit colorChanged(color.toStdString());
        };

        void ProfileItem::handlePopupFinished(ProfileEditPopupResult p)
        {
            if (p.name_valid)
                setName(p.name);
            if (!p.color_name.isEmpty())
                setColor(p.color_name);
        }

        void ProfileItem::mousePressEvent(QMouseEvent* ev)
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

        void ProfileItem::setColorInCSS(QString c)
        {
            css_string.replace(QRegExp("#([0-9]|[a-f]|[A-F]){6}"),c);
        }

        void ProfileItem::updateStyle()
        {
            if (selected)
                setColorInCSS(item_color.name());
            else
                setColorInCSS(QColor("grey").name());
            setStyleSheet(css_string);
        }

        void ProfileItem::decreaseLabelTextSizeIfNecessary(QString s)
        {
            QFont font = name_label->font();
            QRect bounds = name_label->rect();
            QRect font_bounds = QFontMetrics(font).boundingRect(s);
            if (font_bounds.width() > bounds.width() - 2 || font_bounds.height() > bounds.height() - 2)
                font.setPointSize(font.pointSize() - 1);
            name_label->setFont(font);
        }

        QString ProfileItem::elidedTextIfNecessary(QString s)
        {
            QFont font = name_label->font();
            QRect bounds = name_label->rect();
            QRect font_bounds = QFontMetrics(font).boundingRect(s);

            if (font_bounds.width() > bounds.width() -2 || font_bounds.height() > bounds.height() - 2)
                return QFontMetrics(font).elidedText(s,Qt::ElideRight,bounds.width() - 2);
            else
                return s;
        }

        void ProfileItemGroup::addItem(ProfileItem* i)
        {
            i->setGroupPointer(this);
            items.push_back(i);
            connect(i,SIGNAL(itemPressed(ProfileItem*)),this,SLOT(itemPressed(ProfileItem*)));
            connect(i,SIGNAL(nameChanged(std::string)),this,SIGNAL(currentNameChanged(std::string)));
            connect(i,SIGNAL(colorChanged(std::string)),this,SIGNAL(currentColorChanged(std::string)));
        }

        int ProfileItemGroup::currentIndex()
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

        std::string ProfileItemGroup::getCurrentName()
        {
            if (current_profile != nullptr)
                return current_profile->getName();
            else
                return "";
        }

        std::string ProfileItemGroup::getCurrentColor()
        {
            if (current_profile != nullptr)
                return current_profile->getColor().toStdString();
            else
                return "";
        }

        void ProfileItemGroup::setCurrentName(QString n)
        {
            if (current_profile != nullptr)
                current_profile->setName(n);
        }

        void ProfileItemGroup::itemPressed(ProfileItem* i)
        {
            for (auto item : items)
                item->setSelected(false);
            i->setSelected(true);
            current_profile = i;
        }

    }
}
#include "include/profileitem.h"

namespace FreeFit
{
    namespace GUI
    {
        ProfileEditPopup::ProfileEditPopup(QString n, QWidget* parent) : QWidget(parent, Qt::Popup)
        {
            ly = new QGridLayout(this);
            name = new MaterialTextField("Name",this);
            std::regex name_regex("[a-zA-Z\\s]{1,128}");
            auto func_name_regex = [name_regex](std::string s)->bool{return std::regex_match(s,name_regex);};
            name->setValidationFunction(func_name_regex);

            name->setText(n);

            ly->addWidget(name);
        }

        void ProfileEditPopup::closeEvent(QCloseEvent* e)
        {
            QWidget::closeEvent(e);
            emit popupFinished(ProfileEditPopupResult(name->text(),name->validateText()));
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

        ProfileItem::ProfileItem(QString t_name, QWidget* t_parent) : QWidget(t_parent)
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

        void ProfileItem::handlePopupFinished(ProfileEditPopupResult p)
        {
            if (p.name_valid)
                setName(p.name);
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

        void ProfileItem::updateStyle()
        {
            if (selected)
                css_string.replace("color:grey; border: 2px solid grey;","color:red; border: 2px solid red;");
            else
                css_string.replace("color:red; border: 2px solid red;","color:grey; border: 2px solid grey;");
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
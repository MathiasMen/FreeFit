#include "include/profileeditor.h"

namespace FreeFit
{
    namespace GUI
    {
            ProfileSelectionWidget::ProfileSelectionWidget(QWidget* parent)
                : QScrollArea(parent)
            {
                content = new QWidget(this);
                content_ly = new QHBoxLayout(content);
                content->setLayout(content_ly);
                setWidget(content);
                setWidgetResizable(true);
                setFrameShape(QFrame::NoFrame);
                profile_group = new ProfileItemGroup(this);
                connect(profile_group,SIGNAL(currentNameChanged(std::string)),this,SIGNAL(currentNameChanged(std::string)));
                connect(profile_group,SIGNAL(currentColorChanged(std::string)),this,SIGNAL(currentColorChanged(std::string)));
            }

            void ProfileSelectionWidget::addItem(QString profile_name,QString profile_color)
            {
                ProfileItem* m = new ProfileItem(profile_name,profile_color,this);
                content_ly->addWidget(m);
                profile_group->addItem(m);
            }

            int ProfileSelectionWidget::currentIndex()
            {
                return profile_group->currentIndex();
            }

            void ProfileSelectionWidget::selectProfile(int i)
            {
                profile_group->selectProfile(i);
                emit currentColorChanged(getCurrentColor());
            }

            ProfileEditor::ProfileEditor(std::string p_path)
                : MaterialDialog("","Exercises","Workout Settings"),r(p_path),w(p_path)
            {
                connect(getSkipButton(),SIGNAL(clicked()),this,SIGNAL(skiptToWorkoutGeneration()));
                connect(getAcceptButton(), &QPushButton::clicked, this, &QDialog::accept);

                profile_selection = new ProfileSelectionWidget(this);
                connect(profile_selection,SIGNAL(currentNameChanged(std::string)),this,SLOT(currentNameChanged(std::string)));
                connect(profile_selection,SIGNAL(currentColorChanged(std::string)),this,SLOT(currentColorChanged(std::string)));

                for (auto p : r.getProfileList())
                    v_p.push_back(p);
                for (auto p : v_p)
                    profile_selection->addItem(QString::fromStdString(p.getName()),QString::fromStdString(p.getColor()));

                FreeFit::Data::Profile newProfileProfile = FreeFit::Data::Profile::buildNewProfileProfile();
                v_p.push_back(newProfileProfile);
                profile_selection->addItem(QString::fromStdString(newProfileProfile.getName()),QString::fromStdString(newProfileProfile.getColor()));

                profile_selection->selectProfile(0);

                addWidget(profile_selection,0,0);
            }

            void ProfileEditor::accept()
            {
                std::list<FreeFit::Data::Profile> l;
                for(auto p : v_p)
                {
                    if (!p.isNewProfileProfile())
                        l.push_back(p);
                }
                w.createNodeTree(l);
                w.write();
                QDialog::accept();
            }

            FreeFit::Data::Profile ProfileEditor::getCurrentlySelectedData()
            {
                return v_p[profile_selection->currentIndex()];
            }
    }
}
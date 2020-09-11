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
            }

            void ProfileSelectionWidget::addItem(QString profile_name)
            {
                ProfileItem* m = new ProfileItem(profile_name,this);
                content_ly->addWidget(m);
                profile_group->addItem(m);
            }

            int ProfileSelectionWidget::currentIndex()
            {
                return profile_group->currentIndex();
            }

            ProfileEditor::ProfileEditor(std::string p_path)
                : MaterialDialog(),r(p_path),w(p_path)
            {
                ly = new QGridLayout(this);
                this->setLayout(ly);

                profile_selection = new ProfileSelectionWidget(this);

                skip_exercises_button = new ControlButton("Workout Settings",ControlButton::ForwardButton,ControlButton::Primary,this);
                connect(skip_exercises_button,SIGNAL(clicked()),this,SIGNAL(skiptToWorkoutGeneration()));

                next_page_button = new ControlButton("Exercises",ControlButton::ForwardButton,ControlButton::Secondary,this);
                connect(next_page_button, &QPushButton::clicked, this, &QDialog::accept);

                for (auto p : r.getProfileList())
                    v_p.push_back(p);
                for (auto p : v_p)
                    profile_selection->addItem(QString::fromStdString(p.getName()));

                profile_selection->selectProfile(0);

                vertical_spacer = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

                control_layout = new QHBoxLayout();
                horizontal_spacer_control = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
                
                control_layout->addItem(horizontal_spacer_control);
                control_layout->addWidget(next_page_button,0,Qt::AlignLeft);
                control_layout->addWidget(skip_exercises_button,0,Qt::AlignLeft);

                ly->addWidget(profile_selection,0,0);
                ly->addItem(vertical_spacer,3,0);
                ly->addLayout(control_layout,4,0);
            }

            void ProfileEditor::accept()
            {
                v_p.begin()->setName(getName());
                v_p.begin()->setPathToExerciseDB(getExercisesPath());
                std::list<FreeFit::Data::Profile> l;
                for(auto p : v_p)
                    l.push_back(p);
                w.createNodeTree(l);
                w.write();
                QDialog::accept();
            }

            void ProfileEditor::informationChanged()
            {
                FreeFit::Data::Profile& p = v_p[profile_selection->currentIndex()];
                p.setName(profile_selection->getCurrentName());
            }

            FreeFit::Data::Profile ProfileEditor::getCurrentlySelectedData()
            {
                return v_p[profile_selection->currentIndex()];
            }
    }
}
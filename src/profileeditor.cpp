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
            }

            void ProfileSelectionWidget::addItem(QString profile_name)
            {
                ProfileItem* m = new ProfileItem(profile_name,this);
                content_ly->addWidget(m);
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
                path_exercises_xml = new MaterialTextField("Path to Exercises XML",this);
                profile_name = new MaterialTextField("Name",this);

                std::regex exercises_path_regex(".*\\.xml");
                auto func_exercises_regex = [exercises_path_regex](std::string s)->bool{return std::regex_match(s,exercises_path_regex);};
                path_exercises_xml->setValidationFunction(func_exercises_regex);

                std::regex name_regex("[a-zA-Z\\s]{1,128}");
                auto func_name_regex = [name_regex](std::string s)->bool{return std::regex_match(s,name_regex);};
                profile_name->setValidationFunction(func_name_regex);

                skip_exercises_button = new ControlButton("Workout Settings",ControlButton::ForwardButton,ControlButton::Primary,this);
                connect(skip_exercises_button,SIGNAL(clicked()),this,SIGNAL(skiptToWorkoutGeneration()));

                next_page_button = new ControlButton("Exercises",ControlButton::ForwardButton,ControlButton::Secondary,this);
                connect(next_page_button, &QPushButton::clicked, this, &QDialog::accept);

                for (auto p : r.getProfileList())
                    v_p.push_back(p);
                for (auto p : v_p)
                    profile_selection->addItem(QString::fromStdString(p.getName()));

                connect(profile_selection, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedProfileChanged(int)));
                profile_selection->selectProfile(0);
                connect(profile_name, SIGNAL(textEdited(const QString&)),this,SLOT(informationChanged()));
                connect(path_exercises_xml, SIGNAL(textEdited(const QString&)),this,SLOT(informationChanged()));

                vertical_spacer = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

                control_layout = new QHBoxLayout();
                horizontal_spacer_control = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
                
                control_layout->addItem(horizontal_spacer_control);
                control_layout->addWidget(next_page_button,0,Qt::AlignLeft);
                control_layout->addWidget(skip_exercises_button,0,Qt::AlignLeft);

                ly->addWidget(profile_selection,0,0);
                ly->addWidget(path_exercises_xml,1,0);
                ly->addWidget(profile_name,2,0);
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

            void ProfileEditor::selectedProfileChanged(int index)
            {
                profile_name->setText(QString::fromStdString(v_p[index].getName()));
                path_exercises_xml->setText(QString::fromStdString(v_p[index].getPathToExerciseDB()));
            }

            void ProfileEditor::informationChanged()
            {
                FreeFit::Data::Profile& p = v_p[profile_selection->currentIndex()];
                p.setName(profile_name->text().toStdString());
                p.setPathToExerciseDB(path_exercises_xml->text().toStdString());
            }

            FreeFit::Data::Profile ProfileEditor::getCurrentlySelectedData()
            {
                return v_p[profile_selection->currentIndex()];
            }
    }
}
#pragma once 

#include <vector>

#include <QApplication>
#include <QDialog>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QScrollArea>

#include "include/profile.h"
#include "include/xmlreader.h"
#include "include/xmlwriter.h"
#include "include/controls.h"
#include "include/materialtextfield.h"
#include "include/materialbutton.h"
#include "include/materialdialog.h"
#include "include/materialclip.h"

namespace FreeFit
{
    namespace GUI
    {
        class ProfileEditorValidator;

        class ProfileSelection : public QScrollArea
        {
        Q_OBJECT
        public:
            ProfileSelection(QWidget* parent = nullptr) : QScrollArea(parent)
            {
                content = new QWidget(this);
                content_ly = new QHBoxLayout(content);
                content->setLayout(content_ly);
                setWidget(content);
                setWidgetResizable(true);
            }

            void addItem(QString profile_name)
            {
                MaterialClip* m = new MaterialClip(profile_name);
                content_ly->addWidget(m);
                profiles.push_back(m);
                connect(m,SIGNAL(clicked(MaterialClip*)),this,SLOT(deselectOthers(MaterialClip*)));
                connect(m,SIGNAL(clicked(MaterialClip*)),this,SLOT(select(MaterialClip*)));
            }

            int currentIndex()
            {
                int i = -1;
                int c = 0;
                for (auto p : profiles)
                {
                    if (p == current_profile)
                        i = c;
                    c += 1;
                }
                return i;
            }

            void selectProfile(int i)
            {
                select(profiles[i]);
            }
        signals:
            void currentIndexChanged(int);
        private slots:
            void deselectOthers(MaterialClip* c)
            {
                for (auto p : profiles)
                    if (p != c)
                        p->deselect();
            }

            void select(MaterialClip* c)
            {
                current_profile = c;
                c->select();
                emit currentIndexChanged(currentIndex());
            }

        private:
            QWidget* content;
            QHBoxLayout* content_ly;
            std::vector<MaterialClip*> profiles;
            MaterialClip* current_profile;
        };

        class ProfileEditor : public MaterialDialog
        {
        Q_OBJECT
        friend ProfileEditorValidator;
        public:
            ProfileEditor(std::string p_path) : MaterialDialog(),r(p_path),w(p_path)
            {
                ly = new QGridLayout(this);
                this->setLayout(ly);

                profile_selection = new ProfileSelection(this);
                path_exercises_xml = new MaterialTextField("Path to Exercises XML",this);
                profile_name = new MaterialTextField("Name",this);

                next_page_button = new ControlButton("Exercises",ControlButton::ForwardButton,ControlButton::Primary,this);
                connect(next_page_button, &QPushButton::clicked, this, &QDialog::accept);

                skip_exercises_button = new ControlButton("Skip Exercises",ControlButton::ForwardButton,ControlButton::Secondary,this);
                connect(skip_exercises_button,SIGNAL(clicked()),this,SIGNAL(skiptToWorkoutGeneration()));

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
                control_layout->addWidget(skip_exercises_button,0,Qt::AlignLeft);
                control_layout->addWidget(next_page_button,0,Qt::AlignLeft);

                ly->addWidget(profile_selection,0,0);
                ly->addWidget(path_exercises_xml,1,0);
                ly->addWidget(profile_name,2,0);
                ly->addItem(vertical_spacer,3,0);
                ly->addLayout(control_layout,4,0);
            }
        
            std::string getExercisesPath(){return path_exercises_xml->text().toStdString();}

            std::string getName(){return profile_name->text().toStdString();}
        signals:
            void skiptToWorkoutGeneration();
        public slots:

            void accept() override
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

            void selectedProfileChanged(int index)
            {
                profile_name->setText(QString::fromStdString(v_p[index].getName()));
                path_exercises_xml->setText(QString::fromStdString(v_p[index].getPathToExerciseDB()));
            }

            void informationChanged()
            {
                FreeFit::Data::Profile& p = v_p[profile_selection->currentIndex()];
                p.setName(profile_name->text().toStdString());
                p.setPathToExerciseDB(path_exercises_xml->text().toStdString());
            }

            FreeFit::Data::Profile getCurrentlySelectedData()
            {
                return v_p[profile_selection->currentIndex()];
            }

        private:
            std::vector<FreeFit::Data::Profile> v_p;
            FreeFit::Data::ProfileXMLReader r;
            FreeFit::Data::ProfileWriter w;
            QGridLayout* ly;

            ProfileSelection* profile_selection;

            MaterialTextField* path_exercises_xml;
            MaterialTextField* profile_name;

            QSpacerItem* vertical_spacer;

            ControlButton* next_page_button;
            ControlButton* skip_exercises_button;
            QHBoxLayout* control_layout;
            QSpacerItem* horizontal_spacer_control;
        };

        class ProfileEditorValidator
        {
        public:
            ProfileEditorValidator(ProfileEditor* t_p) : p(t_p){}

            int getNumberOfLoadedProfiles(){return p->v_p.size();}
            FreeFit::Data::Profile getProfile(int index){return p->v_p[index];}

            void setName(std::string n){p->profile_name->setText(QString::fromStdString(n));}
            void setXMLOutPath(std::string f){p->w.setOutPath(f);}
        private:
            ProfileEditor* p;
        };
    }
}

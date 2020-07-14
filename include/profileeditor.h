#pragma once 

#include <vector>

#include <QApplication>
#include <QDialog>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QComboBox>

#include "include/profile.h"
#include "include/xmlreader.h"
#include "include/xmlwriter.h"
#include "include/controls.h"
#include "include/materialtextfield.h"
#include "include/materialbutton.h"

namespace FreeFit
{
    namespace GUI
    {
        class ProfileEditorValidator;

        class ProfileEditor : public QDialog
        {
        Q_OBJECT
        friend ProfileEditorValidator;
        public:
            ProfileEditor(std::string p_path) : QDialog(),r(p_path),w(p_path)
            {
                ly = new QGridLayout(this);
                this->setLayout(ly);

                profile_selection = new QComboBox(this);
                path_exercises_xml = new MaterialTextField("Path to Exercises XML",this);
                profile_name = new MaterialTextField("Name",this);

                const int button_width = 140;
                const int button_height = 30;

                next_page_button = new ControlButton("Exercises",ControlButton::ForwardButton,this);
                next_page_button->setMinimumSize(button_width,button_height);
                next_page_button->setMaximumSize(button_width,button_height);
                connect(next_page_button, &QPushButton::clicked, this, &QDialog::accept);

                skip_exercises_button = new ControlButton("Skip Exercises",ControlButton::ForwardButton,this);
                skip_exercises_button->setMinimumSize(button_width,button_height);
                skip_exercises_button->setMaximumSize(button_width,button_height);
                connect(skip_exercises_button,SIGNAL(clicked()),this,SIGNAL(skiptToWorkoutGeneration()));

                for (auto p : r.getProfileList())
                    v_p.push_back(p);
                for (auto p : v_p)
                    profile_selection->addItem(QString::fromStdString(p.getName()));

                connect(profile_selection, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedProfileChanged(int)));
                selectedProfileChanged(0);
                connect(profile_name, SIGNAL(textChanged(const QString&)),this,SLOT(informationChanged()));
                connect(path_exercises_xml, SIGNAL(textChanged(const QString&)),this,SLOT(informationChanged()));

                vertical_spacer = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

                ok_layout = new QHBoxLayout();
                skip_layout = new QHBoxLayout();
                horizontal_spacer_ok = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
                horizontal_spacer_skip = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
                
                ok_layout->addItem(horizontal_spacer_ok);
                ok_layout->addWidget(next_page_button,0,Qt::AlignLeft);

                skip_layout->addItem(horizontal_spacer_skip);
                skip_layout->addWidget(skip_exercises_button,0,Qt::AlignLeft);

                ly->addWidget(profile_selection,0,0);
                ly->addWidget(path_exercises_xml,1,0);
                ly->addWidget(profile_name,2,0);
                ly->addItem(vertical_spacer,3,0);
                ly->addLayout(skip_layout,4,0);
                ly->addLayout(ok_layout,5,0);
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
                path_exercises_xml->setText(QString::fromStdString(v_p[index].getPathToExerciseDB()));
                profile_name->setText(QString::fromStdString(v_p[index].getName()));
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

            QComboBox* profile_selection;

            MaterialTextField* path_exercises_xml;
            MaterialTextField* profile_name;

            QSpacerItem* vertical_spacer;

            ControlButton* next_page_button;
            ControlButton* skip_exercises_button;
            QHBoxLayout* ok_layout;
            QHBoxLayout* skip_layout;
            QSpacerItem* horizontal_spacer_ok;
            QSpacerItem* horizontal_spacer_skip;
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

#pragma once 

#include <vector>

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QComboBox>

#include "include/profile.h"
#include "include/xmlreader.h"
#include "include/xmlwriter.h"

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
                label_path_exercises_xml = new QLabel("Path to Exercises XML:",this);
                label_profile_name = new QLabel("Name:",this);

                path_exercises_xml = new QLineEdit(this);
                profile_name = new QLineEdit(this);

                button_box = new QDialogButtonBox(QDialogButtonBox::Ok);

                for (auto p : r.getProfileList())
                    v_p.push_back(p);
                for (auto p : v_p)
                    profile_selection->addItem(QString::fromStdString(p.getName()));

                connect(profile_selection, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedProfileChanged(int)));
                selectedProfileChanged(0);
                connect(profile_name, SIGNAL(textChanged(const QString&)),this,SLOT(informationChanged()));
                connect(path_exercises_xml, SIGNAL(textChanged(const QString&)),this,SLOT(informationChanged()));
                connect(button_box, &QDialogButtonBox::accepted, this, &ProfileEditor::accept);
                connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

                ly->addWidget(profile_selection,0,0,1,2);
                ly->addWidget(label_path_exercises_xml,1,0);
                ly->addWidget(label_profile_name,2,0);
                ly->addWidget(path_exercises_xml,1,1);
                ly->addWidget(profile_name,2,1);
                ly->addWidget(button_box,3,0,1,2);
            }
        
            std::string getExercisesPath(){return path_exercises_xml->text().toStdString();}

            std::string getName(){return profile_name->text().toStdString();}

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

            QLabel* label_path_exercises_xml;
            QLabel* label_profile_name;

            QLineEdit* path_exercises_xml;
            QLineEdit* profile_name;

            QDialogButtonBox* button_box; 
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

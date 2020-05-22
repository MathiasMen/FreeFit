#pragma once 

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>

#include "include/profile.h"
#include "include/xmlreader.h"

namespace FreeFit
{
    namespace GUI
    {
        class ProfileEditor : public QDialog
        {
        Q_OBJECT
        public:
            ProfileEditor(std::string p_path) : QDialog(),r(p_path)
            {
                ly = new QGridLayout(this);
                this->setLayout(ly);

                r.read();
                p = r.getProfile();

                label_path_exercises_xml = new QLabel("Path to Exercises XML:",this);
                label_profile_name = new QLabel("Name:",this);

                path_exercises_xml = new QLineEdit(this);
                profile_name = new QLineEdit(this);
                path_exercises_xml->setText(QString::fromStdString(p.getPathToExerciseDB()));
                profile_name->setText(QString::fromStdString(p.getName()));

                button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
                connect(button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
                connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

                ly->addWidget(label_path_exercises_xml,0,0);
                ly->addWidget(label_profile_name,1,0);
                ly->addWidget(path_exercises_xml,0,1);
                ly->addWidget(profile_name,1,1);
                ly->addWidget(button_box,2,0,1,2);
            }
        
            std::string getExercisesPath(){return path_exercises_xml->text().toStdString();}

            std::string getName(){return profile_name->text().toStdString();}
        private:
            FreeFit::Data::Profile p;
            FreeFit::Data::ProfileXMLReader r;
            QGridLayout* ly;

            QLabel* label_path_exercises_xml;
            QLabel* label_profile_name;

            QLineEdit* path_exercises_xml;
            QLineEdit* profile_name;

            QDialogButtonBox* button_box; 
        };
    }
}

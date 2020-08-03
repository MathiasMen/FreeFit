#pragma once 

#include <vector>
#include <iostream>

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
        friend ProfileEditorValidator;
        public:
            ProfileSelection(QWidget* parent);
            void addItem(QString profile_name);
            int currentIndex();
            void selectProfile(int i);
        signals:
            void currentIndexChanged(int);
        private slots:
            void deselectOthers(MaterialClip* c);
            void select(MaterialClip* c);
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
            ProfileEditor(std::string p_path);
            std::string getExercisesPath(){return path_exercises_xml->text().toStdString();}
            std::string getName(){return profile_name->text().toStdString();}
        signals:
            void skiptToWorkoutGeneration();
        public slots:
            void accept() override;
            void selectedProfileChanged(int index);
            void informationChanged();
            FreeFit::Data::Profile getCurrentlySelectedData();
        private:
            ProfileSelection* getProfileSelection(){return profile_selection;}
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

        class ProfileEditorValidator : public QObject
        {
        Q_OBJECT
        public:
            ProfileEditorValidator(ProfileEditor* t_p) : p(t_p){}

            int getNumberOfLoadedProfiles(){return p->v_p.size();}
            FreeFit::Data::Profile getProfile(int index){return p->v_p[index];}

            void selectProfile(int i){p->getProfileSelection()->selectProfile(i);};
            void setName(std::string n)
            {
                connect(this,SIGNAL(changeTextSignal(const QString&)),p->profile_name,SIGNAL(textEdited(const QString&)));
                p->profile_name->setText(QString::fromStdString(n));
                emit changeTextSignal(QString::fromStdString(n));
            }
            void setXMLOutPath(std::string f)
            {
                p->w.setOutPath(f);
            }

            std::string getName(){return p->profile_name->text().toStdString();}
            std::string getXMLOutPath(){return p->path_exercises_xml->text().toStdString();}
        signals:
            void changeTextSignal(const QString&);
        private:
            ProfileEditor* p;
        };
    }
}

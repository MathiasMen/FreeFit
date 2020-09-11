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
#include "include/profileitem.h"

namespace FreeFit
{
    namespace GUI
    {
        class ProfileEditorValidator;

        class ProfileSelectionWidget : public QScrollArea
        {
        Q_OBJECT
        friend ProfileEditorValidator;
        public:
            ProfileSelectionWidget(QWidget* parent);
            void addItem(QString profile_name);
            int currentIndex();
            void selectProfile(int i){profile_group->selectProfile(i);}
            std::string getCurrentName(){return profile_group->getCurrentName();};
        signals:
            void currentIndexChanged(int);
        private:
            QWidget* content;
            QHBoxLayout* content_ly;
            ProfileItemGroup* profile_group;
        };

        class ProfileEditor : public MaterialDialog
        {
        Q_OBJECT
        friend ProfileEditorValidator;
        public:
            ProfileEditor(std::string p_path);
            std::string getExercisesPath(){return getCurrentlySelectedData().getPathToExerciseDB();}
            std::string getName(){return getCurrentlySelectedData().getName();}
        signals:
            void skiptToWorkoutGeneration();
        public slots:
            void accept() override;
            void informationChanged();
            FreeFit::Data::Profile getCurrentlySelectedData();
        private:
            ProfileSelectionWidget* getProfileSelection(){return profile_selection;}
            std::vector<FreeFit::Data::Profile> v_p;
            FreeFit::Data::ProfileXMLReader r;
            FreeFit::Data::ProfileWriter w;
            QGridLayout* ly;

            ProfileSelectionWidget* profile_selection;

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
            FreeFit::Data::Profile getSelectedProfileData(){return p->getCurrentlySelectedData();}
            
            void selectProfile(int i){p->getProfileSelection()->selectProfile(i);};
            void setName(std::string n)
            {
/*
                connect(this,SIGNAL(changeTextSignal(const QString&)),p->profile_name,SIGNAL(textEdited(const QString&)));
                p->profile_name->setText(QString::fromStdString(n));
                emit changeTextSignal(QString::fromStdString(n));
*/
            }

            void setXMLOutPath(std::string f)
            {
/*
                connect(this,SIGNAL(changeTextSignal(const QString&)),p->path_exercises_xml,SIGNAL(textEdited(const QString&)));
                p->path_exercises_xml->setText(QString::fromStdString(f));
                emit changeTextSignal(QString::fromStdString(f));
*/
            }
            void changeOutputPath(std::string f)
            {
                p->w.setOutPath(f);
            }

            std::string getName(){return p->profile_selection->getCurrentName();}
            std::string getXMLOutPath(){return "";}
            std::string getProfileCSSString(int i){return "";}

/*
            std::string getName(){return p->profile_name->text().toStdString();}
            std::string getXMLOutPath(){return p->path_exercises_xml->text().toStdString();}
            std::string getProfileCSSString(int i){return p->profile_selection->profile_group->getItems()[i]->styleSheet().toStdString();}
*/
        signals:
            void changeTextSignal(const QString&);
        private:
            ProfileEditor* p;
        };
    }
}

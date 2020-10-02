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
            void addItem(QString profile_name, QString profile_color);
            int currentIndex();
            void selectProfile(int i);
            std::string getCurrentName(){return profile_group->getCurrentName();};
            std::string getCurrentColor(){return profile_group->getCurrentColor();};
        signals:
            void currentIndexChanged(int);
            void currentNameChanged(std::string);
            void currentColorChanged(std::string);
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
            std::string getColor(){return getCurrentlySelectedData().getColor();}
        signals:
            void skiptToWorkoutGeneration();
        public slots:
            void accept() override;
            void informationChanged();
            void currentNameChanged(std::string n){v_p[profile_selection->currentIndex()].setName(n);}
            void currentColorChanged(std::string c)
            {
                v_p[profile_selection->currentIndex()].setColor(c);
                this->setColor(c);
            }

            FreeFit::Data::Profile getCurrentlySelectedData();
        private:
            ProfileSelectionWidget* getProfileSelection(){return profile_selection;}
            std::vector<FreeFit::Data::Profile> v_p;
            FreeFit::Data::ProfileXMLReader r;
            FreeFit::Data::ProfileWriter w;

            ProfileSelectionWidget* profile_selection;

            QSpacerItem* vertical_spacer;

            QSpacerItem* horizontal_spacer_control;
        };

        class ProfileEditorValidator : public QObject
        {
        Q_OBJECT
        public:
            ProfileEditorValidator(ProfileEditor* t_p) : p(t_p){}

            int getNumberOfLoadedProfiles(){return p->v_p.size();}

            ProfileItem* getCurrentSelectionItem()
            {
                return p->profile_selection->profile_group->getItems()[p->profile_selection->currentIndex()];
            }

            FreeFit::Data::Profile getProfileData(int index){return p->v_p[index];}

            FreeFit::Data::Profile getSelectedProfileData(){return p->getCurrentlySelectedData();}
            
            void selectProfile(int i){p->getProfileSelection()->selectProfile(i);};

            void setCurrentName(std::string n)
            {
                getCurrentSelectionItem()->setName(QString::fromStdString(n));
            }

            void setCurrentColor(std::string c)
            {
                getCurrentSelectionItem()->setColor(QString::fromStdString(c));
            }

            void changeOutputPath(std::string f)
            {
                p->w.setOutPath(f);
            }

            std::string getCurrentName(){return p->profile_selection->getCurrentName();}

            std::string getCurrentColor(){return p->profile_selection->getCurrentColor();}

            std::string getCurrentProfileCSSString(int i){return getCurrentSelectionItem()->styleSheet().toStdString();}

            std::string getNextButtonCSS(){return p->getAcceptButton()->styleSheet().toStdString();}

            std::string getSkipButtonCSS(){return p->getSkipButton()->styleSheet().toStdString();}
        signals:
            void changeTextSignal(const QString&);
        private:
            ProfileEditor* p;
        };
    }
}

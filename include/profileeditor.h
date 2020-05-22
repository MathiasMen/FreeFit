#pragma once 

#include <QDialog>

#include "include/profile.h"

namespace FreeFit
{
    namespace GUI
    {
        class ProfileEditor : public QDialog
        {
        public:
            ProfileEditor(std::string p_path)
            {

            }
        private:
            FreeFit::Data::Profile p;
        };
    }
}

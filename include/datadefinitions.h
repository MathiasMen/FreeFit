#pragma once

#include <string>

namespace FreeFit
{
    namespace Data
    {
        enum MuscleGroup {Shoulder, Back, Chest, Abs, Arms, Legs, Error};

        static FreeFit::Data::MuscleGroup stringToMuscleGroup(std::string s)
        {
            if (s == "Shoulder")
                return FreeFit::Data::MuscleGroup::Shoulder;
            else if (s == "Back")
                return FreeFit::Data::MuscleGroup::Back;
            else if (s == "Chest")
                return FreeFit::Data::MuscleGroup::Chest;
            else if (s == "Abs")
                return FreeFit::Data::MuscleGroup::Abs;
            else if (s == "Arms")
                return FreeFit::Data::MuscleGroup::Arms;
            else if (s == "Legs")
                return FreeFit::Data::MuscleGroup::Legs;
            else
                return FreeFit::Data::MuscleGroup::Error;
        }
    }
}
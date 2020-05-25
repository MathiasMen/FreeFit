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

        static std::string muscleGroupToString(FreeFit::Data::MuscleGroup m)
        {
            if (m == FreeFit::Data::MuscleGroup::Shoulder)
                return "Shoulder";
            else if (m == FreeFit::Data::MuscleGroup::Back)
                return "Back";
            else if (m == FreeFit::Data::MuscleGroup::Chest)
                return "Chest";
            else if (m == FreeFit::Data::MuscleGroup::Abs)
                return "Abs";
            else if (m == FreeFit::Data::MuscleGroup::Arms)
                return "Arms";
            else if (m == FreeFit::Data::MuscleGroup::Legs)
                return "Legs";
            else
                return "";
        }
    }
}
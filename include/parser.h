#pragma once

#include <list>
#include <memory>
#include <string>
#include "include/exercise.h"
#include "include/profile.h"
#include "include/xmlnode.h"

namespace FreeFit
{
    namespace Data
    {
        class ExerciseTreeParser
        {
            public:
                ExerciseTreeParser();
                std::list<Exercise> parse(std::shared_ptr<XMLNode> root);
        };

        class ProfileTreeParser
        {
            public:
                ProfileTreeParser();
                Profile parse(std::shared_ptr<XMLNode> root);
        };
    }
}
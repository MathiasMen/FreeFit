#pragma once

#include <list>
#include <memory>
#include <string>
#include "include/exercise.h"
#include "include/xmlnode.h"

namespace FreeFit
{
    namespace Data
    {
        class ExerciseTreeParser
        {
            public:
                ExerciseTreeParser(){}
                std::list<Exercise> parse(std::shared_ptr<XMLNode> root)
                {
                    std::list<Exercise> l;
                    std::list<std::shared_ptr<XMLNode>> exercises = root->findAllChildren("EXERCISE");
                    for (auto e_ptr : exercises)
                    {
                        Exercise e;
                        if(e_ptr->findFirstChild("NAME"))
                            e.setName(e_ptr->findFirstChild("NAME")->getValue());

                        if(e_ptr->findFirstChild("VIDEOPATH"))
                            e.setVideoPath(e_ptr->findFirstChild("VIDEOPATH")->getValue());

                        if(e_ptr->findFirstChild("BASEVOL"))
                            e.setBaseVolume(std::stoi(e_ptr->findFirstChild("BASEVOL")->getValue()));

                        if(e_ptr->findFirstChild("TYPE"))
                            e.setExerciseType(static_cast<ExerciseType>(std::stoi(e_ptr->findFirstChild("TYPE")->getValue())));

                        if(e_ptr->findFirstChild("TRAINEDAREAS"))
                            for(auto a : e_ptr->findFirstChild("TRAINEDAREAS")->findAllChildren("AREA"))
                                e.addTrainedMuscle(static_cast<MuscleGroup>(std::stoi(a->getValue())));
                        l.push_back(e);
                    }
                    return l;
                }
        };
/*
        class ProfileTreeParser
        {

        };
*/
    }
}
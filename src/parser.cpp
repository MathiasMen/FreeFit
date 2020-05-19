#include "include/parser.h"

namespace FreeFit
{
    namespace Data
    {
        ExerciseTreeParser::ExerciseTreeParser(){}
        std::list<Exercise> ExerciseTreeParser::parse(std::shared_ptr<XMLNode> root)
        {
            std::list<Exercise> l;
            std::list<std::shared_ptr<XMLNode>> exercises = root->findAllChildren("EXERCISE");
            for (auto e_ptr : exercises)
            {
                Exercise e;
                if(e_ptr->findFirstChild("NAME"))
                    e.setName(e_ptr->findFirstChild("NAME")->getValue());
                
                if(e_ptr->findFirstChild("VIDEOURL"))
                    e.setVideoURL(e_ptr->findFirstChild("VIDEOURL")->getValue());

                if(e_ptr->findFirstChild("VIDEOPATH"))
                    e.setVideoPath(e_ptr->findFirstChild("VIDEOPATH")->getValue());

                if(e_ptr->findFirstChild("VIDEOSTARTTIME"))
                    e.setVideoStartTime(e_ptr->findFirstChild("VIDEOSTARTTIME")->getValue());

                if(e_ptr->findFirstChild("VIDEOENDTIME"))
                    e.setVideoEndTime(e_ptr->findFirstChild("VIDEOENDTIME")->getValue());

                if(e_ptr->findFirstChild("TRAINEDAREAS"))
                    for(auto a : e_ptr->findFirstChild("TRAINEDAREAS")->findAllChildren("AREA"))
                        e.addTrainedMuscle(static_cast<MuscleGroup>(std::stoi(a->getValue())));
                l.push_back(e);
            }
            return l;
        }


        ProfileTreeParser::ProfileTreeParser(){}
        Profile ProfileTreeParser::parse(std::shared_ptr<XMLNode> root)
        {
            Profile p;
            if(root->findFirstChild("NAME"))
                p.setName(root->findFirstChild("NAME")->getValue());

            if(root->findFirstChild("PICTUREPATH"))
                p.setPicturePath(root->findFirstChild("PICTUREPATH")->getValue());

            if(root->findFirstChild("DATELASTWORKOUT"))
                p.setDateLastWorkout(root->findFirstChild("DATELASTWORKOUT")->getValue());

            if(root->findFirstChild("PATHEXDB"))
                p.setPathToExerciseDB(root->findFirstChild("PATHEXDB")->getValue());

            if(root->findFirstChild("PERFFACTOR"))
                p.setPerformanceFactor(std::stod(root->findFirstChild("PERFFACTOR")->getValue()));
            return p;
        }
    }
}
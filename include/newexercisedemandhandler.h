#pragma once

#include <memory>
#include <queue>

#include "include/xmlreader.h"
#include "include/xmlwriter.h"
#include "include/newexercisedemand.h"
#include "include/exercise.h"

namespace FreeFit
{
    namespace Data
    {
        class NewExerciseDemandHandler
        {
        public:
            NewExerciseDemandHandler(std::string t_path_to_db):path_to_db(t_path_to_db)
            {
                r = std::make_shared<BaseXMLReader>(path_to_db);
                w = std::make_shared<ExerciseWriter>(path_to_db);
            }

            void addDemand(std::shared_ptr<GUI::NewExerciseDemand> d)
            {
                demands.push(d);
            }

            void executeDemands()
            {
                w->copyNodeTree(r->read());
            }

            void downloadVideo(std::string url, std::string start_time, std::string stop_time)
            {

            }

            FreeFit::Data::MuscleGroup stringToMuscleGroup(std::string s)
            {
                switch(s)
                {
                    case "Shoulder":
                        return FreeFit::Data::MuscleGroup::Shoulder;
                    case "UpperBack":
                        return FreeFit::Data::MuscleGroup::UpperBack;
                    case "MiddleBack":
                        return FreeFit::Data::MuscleGroup::MiddleBack;
                    case "LowerBack":
                        return FreeFit::Data::MuscleGroup::LowerBack;
                    case "Chest":
                        return FreeFit::Data::MuscleGroup::Chest;
                    case "Biceps":
                        return FreeFit::Data::MuscleGroup::Biceps;
                    case "Triceps":
                        return FreeFit::Data::MuscleGroup::Triceps;
                    case "Forearms":
                        return FreeFit::Data::MuscleGroup::Forearms;
                    case "CoreAbs":
                        return FreeFit::Data::MuscleGroup::CoreAbs;
                    case "SideAbs":
                        return FreeFit::Data::MuscleGroup::SideAbs;
                    case "Glutes":
                        return FreeFit::Data::MuscleGroup::Glutes;
                    case "Thigh":
                        return FreeFit::Data::MuscleGroup::Thigh;
                    case "Harmstrings":
                        return FreeFit::Data::MuscleGroup::Harmstrings;
                    case "Tibia":
                        return FreeFit::Data::MuscleGroup::Tibia;
                    case "Calves":
                        return FreeFit::Data::MuscleGroup::Calves;
                    default:
                        return FreeFit::Data::MuscleGroup::Error;
                }
            }
        private:
            std::queue<std::shared_ptr<GUI::NewExerciseDemand>> demands;
            std::string path_to_db;
            std::shared_ptr<BaseXMLReader> r;
            std::shared_ptr<ExerciseWriter> w;
        };
    }
}
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

        private:
            std::queue<std::shared_ptr<GUI::NewExerciseDemand>> demands;
            std::string path_to_db;
            std::shared_ptr<BaseXMLReader> r;
            std::shared_ptr<ExerciseWriter> w;
        };
    }
}
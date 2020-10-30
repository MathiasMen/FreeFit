#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <list>
#include <memory>
#include <set>
#include "include/xmlnode.h"
#include "include/exercise.h"
#include "include/profile.h"
#include "include/workout.h"

namespace FreeFit
{
    namespace Data
    {
        class BaseXMLWriter
        {
        public:
            BaseXMLWriter(std::string f) : filepath(f){};

            void write();

            void setOutPath(std::string f){filepath = f;}
        protected:
            std::string writeXMLString(std::shared_ptr<XMLNode> n);
            std::string formatXMLString(std::string s_xml);
            void indent_string(std::string& s,int indent_level);
            void writeFile(std::string f, std::string c);

            std::string filepath;
            std::shared_ptr<XMLNode> root;
        };

        class ExerciseWriter : public BaseXMLWriter
        {
        public:
            ExerciseWriter(std::string filepath) : BaseXMLWriter(filepath){}

            std::shared_ptr<XMLNode> exerciseToNode(FreeFit::Data::Exercise ex);
            void addExercisesToNodeTree(std::list<FreeFit::Data::Exercise> l_e);
            void createNodeTree(std::list<FreeFit::Data::Exercise> l_e);
            void copyNodeTree(std::shared_ptr<XMLNode> new_root);
        };

        class ProfileWriter : public BaseXMLWriter
        {
        public:
            ProfileWriter(std::string filepath) : BaseXMLWriter(filepath){}

            std::shared_ptr<XMLNode> profileToNode(FreeFit::Data::Profile p);
            void addProfilesToNodeTree(std::list<FreeFit::Data::Profile> l_p);
            void createNodeTree(std::list<FreeFit::Data::Profile> l_p);
        };

        class WorkoutWriter : public BaseXMLWriter
        {
        public:
            WorkoutWriter(std::string filepath) : BaseXMLWriter(filepath){}

            std::shared_ptr<XMLNode> workoutToNode(FreeFit::Data::WorkoutBase* workout);
            void addWorkoutsToNodeTree(std::list<FreeFit::Data::WorkoutBase*> l_w);
            void createNodeTree(std::list<FreeFit::Data::WorkoutBase*> l_w);
        };
    }
}

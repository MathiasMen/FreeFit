#pragma once

#include <memory>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

#include "include/xmlnode.h"
#include "include/exercise.h"
#include "include/parser.h"
#include "include/profile.h"

namespace FreeFit
{
    namespace Data
    {
        class BaseXMLReader
        {
        public:
            BaseXMLReader(std::string f);

            std::shared_ptr<XMLNode> read();

            void setInPath(std::string f){filepath = f;}
        private:

            void createNodeTree();

            std::shared_ptr<XMLNode> XMLStringToData(std::string s,std::shared_ptr<XMLNode> p = nullptr);
            std::string getNodeNameFromCurrentString(std::string n, int offset = 0);
            std::string getNodeValueFromCurrentString(std::string n);
            std::list<std::string> extractChildrenStrings(std::string s,std::string parent_node_name);
            bool isStringXMLNode(std::string);
            void removeSubstring(std::string& str, const std::string& sub_str);

            std::string filepath;
            std::shared_ptr<XMLNode> root;
        };

        class ExerciseXMLReader : public BaseXMLReader
        {
        public:
            ExerciseXMLReader(std::string f):BaseXMLReader(f){}
            std::list<Exercise> getExerciseList();
        };

        class ProfileXMLReader : public BaseXMLReader
        {
        public:
            ProfileXMLReader(std::string f):BaseXMLReader(f){}
            Profile getProfile();
        };        
    }
}

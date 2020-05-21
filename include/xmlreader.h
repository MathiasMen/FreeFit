#pragma once

#include <memory>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

#include "include/xmlnode.h"
#include "include/exercise.h"
#include "include/parser.h"

namespace FreeFit
{
    namespace Data
    {
        class BaseXMLReader
        {
        public:
            BaseXMLReader(std::string f);

            std::shared_ptr<XMLNode> read();

            std::list<Exercise> getExerciseList()
            {
                std::shared_ptr<XMLNode> n = read();
                if(n)
                {
                    ExerciseTreeParser p;
                    return p.parse(n);
                }
                else
                    return std::list<Exercise>();
            }
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
    }
}

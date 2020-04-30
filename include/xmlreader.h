#pragma once

#include <memory>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#include "include/xmlnode.h"

namespace FreeFit
{
    namespace Data
    {
        class BaseXMLReader
        {
        public:
            BaseXMLReader(std::string f);

            std::shared_ptr<XMLNode> read();

        private:

            void createNodeTree();

            std::shared_ptr<XMLNode> parseXMLString(std::string s,std::shared_ptr<XMLNode> p = nullptr);
            std::string findNodeName(std::string n, int offset = 0);
            std::string findNodeValue(std::string n);
            std::list<std::string> extractChildrenStrings(std::string s,std::string parent_node_name);
            void removeSubstring(std::string& str, const std::string& sub_str);

            std::string filepath;
            std::shared_ptr<XMLNode> root;
        };        
    }
}

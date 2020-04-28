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
            BaseXMLReader(std::string f):filepath(f){}
            std::shared_ptr<XMLNode> read()
            {
                createNodeTree();
                return root;
            }
        private:

            void createNodeTree()
            {
                std::ifstream in(filepath);
                std::string in_content((std::istreambuf_iterator<char>(in)),std::istreambuf_iterator<char>());
                root = parseXMLString(in_content);
            }

            std::shared_ptr<XMLNode> parseXMLString(std::string s,std::shared_ptr<XMLNode> p = nullptr)
            {
                std::string node_name = findNodeName(s);
                std::string node_value = findNodeValue(s);
                std::shared_ptr<XMLNode> n = std::make_shared<XMLNode>(nullptr,node_name,node_value);
                for(std::string c : findChildrenStrings(s))
                {
                    std::cout << c << std::endl;
                    n->addChild(parseXMLString(c,n));
                }
                return n;
            }

            std::string findNodeName(std::string n, int offset = 0)
            {
                size_t name_tag_start = n.find_first_of("<",offset);
                size_t name_tag_end = n.find_first_of(">",offset);
                size_t name_length = name_tag_end - name_tag_start - 1; // Offset of 1 due to zero-based array.
                return n.substr(name_tag_start+1,name_length);
            }

            std::string findNodeValue(std::string n)
            {
                // Search and extract the second line (after the name-tag).
                size_t val_start = n.find_first_of("\n") + 1;
                size_t val_end = n.find_first_of("\n",val_start);
                std::string l2 = n.substr(val_start,val_end - val_start - 1);
                
                // Return empty string if line two is just another name-tag.
                if(l2.find("<") != std::string::npos)
                    return "";
                // Else return string without whitespaces.
                else
                {
                    removeSubstring(l2," ");
                    return l2;
                }
            }

            std::list<std::string> findChildrenStrings(std::string n)
            {
                std::list<std::string> children_strings;
                removeSubstring(n," ");
                removeFirstAndLastLine(n);
                removeSubstring(n,"\n");   
                while(!n.empty())
                {
                    std::string node_name = findNodeName(n);
                    std::string node_start_tag = "<" + node_name + ">";
                    std::string node_end_tag = "</" + node_name + ">";

                    size_t node_start = n.find(node_start_tag);
                    size_t test = n.find(node_end_tag);
                    size_t node_end = n.find(node_end_tag) + node_end_tag.length();
                    std::string node_content = n.substr(node_start,node_end - node_start);
                    children_strings.push_back(node_content);
                    removeSubstring(n,node_content);
                }
                return children_strings;
            }

            void removeSubstring(std::string& str, const std::string& sub_str)
            {
                size_t pos = std::string::npos;
                while ((pos  = str.find(sub_str) )!= std::string::npos)
                    str.erase(pos, sub_str.length());
            }

            void removeFirstAndLastLine(std::string& n)
            {
                size_t first_line_start = 0;
                size_t first_line_end = n.find("\n");

                size_t last_line_end = n.length()-1;
                size_t last_line_start = n.rfind("\n",last_line_end - 1);

                n = n.substr(first_line_end+1,last_line_start - (first_line_end + 1));
            }

            std::string filepath;
            std::shared_ptr<XMLNode> root;
        };        
    }
}

/*
    std::string expected = 
    "<PROFILE>\n"
    "  <NAME>\n"
    "    TestProfile\n"
    "  </NAME>\n"
    "  <PICTUREPATH>\n"
    "    DummyPicturePath\n"
    "  </PICTUREPATH>\n"
    "  <DATELASTWORKOUT>\n"
    "    24.03.1994\n"
    "  </DATELASTWORKOUT>\n"
    "  <PATHEXDB>\n"
    "    DummyExercisesPath\n"
    "  </PATHEXDB>\n"
    "  <PERFFACTOR>\n"
    "    10.000000\n"
    "  </PERFFACTOR>\n"
    "</PROFILE>\n";
*/

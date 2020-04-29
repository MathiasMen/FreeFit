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
                removeSubstring(in_content," ");
                removeSubstring(in_content,"\n");
                removeSubstring(in_content,"\t");
                root = parseXMLString(in_content);
            }

            std::shared_ptr<XMLNode> parseXMLString(std::string s,std::shared_ptr<XMLNode> p = nullptr)
            {
                std::string node_name = findNodeName(s);
                std::string node_value = findNodeValue(s);
                std::shared_ptr<XMLNode> n = std::make_shared<XMLNode>(nullptr,node_name,node_value);

                for(std::string c : extractChildrenStrings(s,node_name))
                    n->addChild(parseXMLString(c,n));

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
                size_t start_tag_end = n.find_first_of(">");
                size_t end_tag_start = n.find_first_of("<",start_tag_end);
                size_t val_len = end_tag_start - start_tag_end - 1;

                if(val_len > 0)
                    return n.substr(start_tag_end+1,val_len);
                else
                    return "";
            }

            std::list<std::string> extractChildrenStrings(std::string s,std::string parent_node_name)
            {
                std::list<std::string> children_strings;

                removeSubstring(s,"<"+parent_node_name+">");
                removeSubstring(s,"</"+parent_node_name+">");

                if(s.find("<") == std::string::npos)
                    return std::list<std::string>();

                while(!s.empty())
                {
                    std::string node_name = findNodeName(s);
                    std::string node_start_tag = "<" + node_name + ">";
                    std::string node_end_tag = "</" + node_name + ">";

                    size_t node_content_start = s.find(node_start_tag);
                    size_t node_content_end = s.find(node_end_tag) + node_end_tag.length();
                    std::string node_content = s.substr(node_content_start, node_content_end - node_content_start);
                    children_strings.push_back(node_content);
                    removeSubstring(s,node_content);
                }
                return children_strings;
            }

            void removeSubstring(std::string& str, const std::string& sub_str)
            {
                size_t pos = std::string::npos;
                while ((pos  = str.find(sub_str) )!= std::string::npos)
                    str.erase(pos, sub_str.length());
            }

            std::string filepath;
            std::shared_ptr<XMLNode> root;
        };        
    }
}
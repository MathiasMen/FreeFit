#pragma once

#include <string>
#include <list>
#include <memory>

namespace FreeFit
{
    namespace Data
    {
        class XMLNode
        {
        public:
            XMLNode(std::shared_ptr<XMLNode> t_parent, std::string t_name, std::string t_value);

            void addChild(std::shared_ptr<XMLNode> n){children.push_back(n);}
            std::shared_ptr<XMLNode> findFirstChild(std::string child_name);
            std::list<std::shared_ptr<XMLNode>> findAllChildren(std::string child_name);
            void setValue(std::string v){value = v;}
            void getXMLString(std::string& res);
            
        private:
            XMLNode();

            std::shared_ptr<XMLNode> parent;
            std::list<std::shared_ptr<XMLNode>> children;
            std::string name;
            std::string value;
        };
    }
}
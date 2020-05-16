#pragma once

#include <string>
#include <list>
#include <memory>
#include <iostream>

namespace FreeFit
{
    namespace Data
    {
        class XMLNode
        {
        public:
            XMLNode(std::shared_ptr<XMLNode> t_parent, std::string t_name, std::string t_value);

            inline bool operator==(const XMLNode& rhs)
            {
                bool childrenEqual = true;
                for (auto c : children)
                {
                    bool isNodeAlreadyInChildren = false;
                    for (auto c_rhs : rhs.children)
                        if (*c == *c_rhs)
                        {
                            isNodeAlreadyInChildren = true;
                            break;
                        }
                    if (!isNodeAlreadyInChildren)
                    {
                        childrenEqual = false;
                        break;
                    }
                }
                return (childrenEqual && this->name == rhs.name && this->value == rhs.value);
            }

            inline bool operator!=(const XMLNode& rhs)
            {
                return !(*this == rhs);
            }

            void addChild(std::shared_ptr<XMLNode> n)
            {
                bool isInChildren = false;
                for (auto c : children)
                    if (*c == *n)
                        isInChildren = true;
                
                if (!isInChildren)
                    children.push_back(n);
            }

            std::shared_ptr<XMLNode> findFirstChild(std::string child_name);
            std::list<std::shared_ptr<XMLNode>> findAllChildren(std::string child_name);
            void setValue(std::string v){value = v;}

            std::string getValue(){return value;}
            std::string getName(){return name;}
            std::list<std::shared_ptr<XMLNode>> getChildren(){return children;}
        private:
            XMLNode();

            std::shared_ptr<XMLNode> parent;
            std::list<std::shared_ptr<XMLNode>> children;
            std::string name;
            std::string value;
        };
    }
}
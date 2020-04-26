#include "include/xmlnode.h"

namespace FreeFit
{
    namespace Data
    {
            XMLNode::XMLNode(std::shared_ptr<XMLNode> t_parent, std::string t_name, std::string t_value)
                : parent(t_parent),name(t_name),value(t_value){}
            
            std::shared_ptr<XMLNode> XMLNode::findFirstChild(std::string child_name)
            {
                for (std::shared_ptr<XMLNode> c : children)
                    if(c->name == child_name)
                        return c;
                return nullptr;
            }

            std::list<std::shared_ptr<XMLNode>> XMLNode::findAllChildren(std::string child_name)
            {
                std::list<std::shared_ptr<XMLNode>> l;
                for (std::shared_ptr<XMLNode> c : children)
                    if(c->name == child_name)
                        l.push_back(c);
                return l;
            }

            void XMLNode::getXMLString(std::string& res)
            {
                res += "<"  + name + ">\n";
                res += (value.empty() ? "" : value + "\n");
                for(std::shared_ptr<XMLNode> c : children)
                    c->getXMLString(res);
                res += "</" + name + ">\n";
            }

    }
}
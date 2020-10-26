#include "include/xmlreader.h"

namespace FreeFit
{
    namespace Data
    {
        BaseXMLReader::BaseXMLReader(std::string f):filepath(f){}

        std::shared_ptr<XMLNode> BaseXMLReader::read()
        {
            createNodeTree();
            return root;
        }

        void BaseXMLReader::createNodeTree()
        {
            std::ifstream in(filepath);
            if(!in.is_open())
            {
                root = nullptr;
                return;
            }
            std::string in_content((std::istreambuf_iterator<char>(in)),std::istreambuf_iterator<char>());
            removeSubstring(in_content," ");
            removeSubstring(in_content,"\n");
            removeSubstring(in_content,"\t");
            root = XMLStringToData(in_content);
        }

        std::shared_ptr<XMLNode> BaseXMLReader::XMLStringToData(std::string s,std::shared_ptr<XMLNode> p)
        {
            std::string node_name = getNodeNameFromCurrentString(s);
            std::string node_value = getNodeValueFromCurrentString(s);
            std::shared_ptr<XMLNode> n = std::make_shared<XMLNode>(nullptr,node_name,node_value);

            for(std::string c : extractChildrenStrings(s,node_name))
                n->addChild(XMLStringToData(c,n));

            return n;
        }

        std::string BaseXMLReader::getNodeNameFromCurrentString(std::string n, int offset)
        {
            size_t name_tag_start = n.find_first_of("<",offset);
            size_t name_tag_end = n.find_first_of(">",offset);
            size_t name_length = name_tag_end - name_tag_start - 1; // Offset of 1 due to zero-based array.
            return n.substr(name_tag_start+1,name_length);
        }

        std::string BaseXMLReader::getNodeValueFromCurrentString(std::string n)
        {
            size_t start_tag_end = n.find_first_of(">");
            size_t end_tag_start = n.find_first_of("<",start_tag_end);
            size_t val_len = end_tag_start - start_tag_end - 1;

            if(val_len > 0)
                return n.substr(start_tag_end+1,val_len);
            else
                return "";
        }

        std::list<std::string> BaseXMLReader::extractChildrenStrings(std::string s,std::string parent_node_name)
        {
            std::list<std::string> children_strings;

            removeSubstring(s,"<"+parent_node_name+">");
            removeSubstring(s,"</"+parent_node_name+">");

            if(!isStringXMLNode(s))
                return std::list<std::string>();

            while(!s.empty())
            {
                std::string node_name = getNodeNameFromCurrentString(s);
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

        bool BaseXMLReader::isStringXMLNode(std::string s)
        {
            return (s.find("<") != std::string::npos);
        }

        void BaseXMLReader::removeSubstring(std::string& str, const std::string& sub_str)
        {
            size_t pos = std::string::npos;
            while ((pos  = str.find(sub_str) )!= std::string::npos)
                str.erase(pos, sub_str.length());
        }

        std::list<Exercise> ExerciseXMLReader::getExerciseList()
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

        std::list<Profile> ProfileXMLReader::getProfileList()
        {
            std::list<Profile> l;
            ProfileTreeParser p;
            std::shared_ptr<XMLNode> r = read();
            for ( auto n : r->findAllChildren("PROFILE"))
                l.push_back(p.parse(n));
            
            return l;
        }

        std::list<CustomExercisesWorkout> WorkoutXMLReader::getWorkoutList()
        {
            std::list<CustomExercisesWorkout> l;
            WorkoutTreeParser p;
            std::shared_ptr<XMLNode> r = read();
            for (auto w : r->findAllChildren("WORKOUT"))
                l.push_back(p.parse(w));

            return l;
        }
    }
}
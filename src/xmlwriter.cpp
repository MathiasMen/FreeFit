#include "include/xmlwriter.h"

namespace FreeFit
{
    namespace Data
    {
        void BaseXMLWriter::write()
        {
            std::string content;
            content = writeXMLString(root);
            content = formatXMLString(content);
            writeFile(filepath,content);
        }

        std::string BaseXMLWriter::writeXMLString(std::shared_ptr<XMLNode> n)
        {
                std::string res;
                res += "<"  + n->getName() + ">\n";
                res += (n->getValue().empty() ? "" : n->getValue() + "\n");
                for(std::shared_ptr<XMLNode> c : n->getChildren())
                    res += writeXMLString(c);
                res += "</" + n->getName() + ">\n";
                return res;
        }

        std::string BaseXMLWriter::formatXMLString(std::string s_xml)
        {
            std::stringstream ss(s_xml);
            std::string line;
            std::string master;
            unsigned int indent_level = 0;
            while(std::getline(ss,line,'\n'))
            {
                if(line.find("</") != std::string::npos)
                {
                    indent_level -= 1;
                    indent_string(line,indent_level);
                }
                else if (line.find("<") != std::string::npos)
                {
                    indent_string(line,indent_level);
                    indent_level += 1;
                }
                else
                    indent_string(line,indent_level);
                
                master.append(line);
                master.append("\n");
            }

            return master;
        }

        void BaseXMLWriter::indent_string(std::string& s,int indent_level)
        {
            s.insert(0,indent_level*2,' ');
        }

        void BaseXMLWriter::writeFile(std::string f, std::string c)
        {
            std::ofstream out(f, std::ofstream::trunc);
            out << c;
            out.close();
        }

        std::shared_ptr<XMLNode> ExerciseWriter::exerciseToNode(FreeFit::Data::Exercise ex)
        {
            std::shared_ptr<XMLNode> e = std::make_shared<XMLNode>(root,"EXERCISE","");
            std::shared_ptr<XMLNode> e_name = std::make_shared<XMLNode>(e,"NAME",ex.getName());
            std::shared_ptr<XMLNode> e_video_url = std::make_shared<XMLNode>(e,"VIDEOURL",ex.getVideoURL());
            std::shared_ptr<XMLNode> e_video_path = std::make_shared<XMLNode>(e,"VIDEOPATH",ex.getVideoPath());
            std::shared_ptr<XMLNode> e_areas = std::make_shared<XMLNode>(e,"TRAINEDAREAS","");
            for (auto m : ex.getTrainedMuscles())
            {
                std::shared_ptr<XMLNode> e_area = std::make_shared<XMLNode>(e_areas,"AREA",std::to_string(m));
                e_areas->addChild(e_area);
            }
            e->addChild(e_name);
            e->addChild(e_video_url);
            e->addChild(e_video_path);
            e->addChild(e_areas);
            return e;
        }

        void ExerciseWriter::addExercisesToNodeTree(std::list<FreeFit::Data::Exercise> l_e)
        {
            for (FreeFit::Data::Exercise e_data : l_e)
            {
                std::shared_ptr<XMLNode> e = exerciseToNode(e_data);
                root->addChild(e);
            }
        }

        void ExerciseWriter::createNodeTree(std::list<FreeFit::Data::Exercise> l_e)
        {
            root = std::make_shared<XMLNode>(nullptr,"EXERCISES","");
            addExercisesToNodeTree(l_e);
        }

        void ExerciseWriter::copyNodeTree(std::shared_ptr<XMLNode> new_root)
        {
            root = new_root;
        }

        void ProfileWriter::createNodeTree(Profile p)
        {
            root = std::make_shared<XMLNode>(nullptr,"PROFILE","");
            std::shared_ptr<XMLNode> p_name = std::make_shared<XMLNode>(root,"NAME",p.getName());
            std::shared_ptr<XMLNode> p_picture_path = std::make_shared<XMLNode>(root,"PICTUREPATH",p.getPicturePath());
            std::shared_ptr<XMLNode> p_date_of_last_workout = std::make_shared<XMLNode>(root,"DATELASTWORKOUT",p.getDateLastWorkout());
            std::shared_ptr<XMLNode> p_path_to_exercise_database = std::make_shared<XMLNode>(root,"PATHEXDB",p.getPathToExerciseDB());
            std::shared_ptr<XMLNode> p_perf_factor = std::make_shared<XMLNode>(root,"PERFFACTOR",std::to_string(p.getPerformanceFactor()));
            root->addChild(p_name);
            root->addChild(p_picture_path);
            root->addChild(p_date_of_last_workout);
            root->addChild(p_path_to_exercise_database);
            root->addChild(p_perf_factor);
        }
    }
}
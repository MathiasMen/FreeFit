#include "include/xmlwriter.h"

namespace FreeFit
{
    namespace Data
    {
        void BaseXMLWriter::write()
        {
            std::string content;
            root->getXMLString(content);
            content = formatXMLString(content);
            writeFile(filepath,content);
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
            std::ofstream out(f);
            out << c;
            out.close();
        }

        void ExerciseWriter::createNodeTree(std::list<FreeFit::Data::Exercise> l_e)
        {
            root = std::make_shared<XMLNode>(nullptr,"ROOT","");
            for (FreeFit::Data::Exercise e_data : l_e)
            {
                std::shared_ptr<XMLNode> e = std::make_shared<XMLNode>(root,"EXERCISE","");
                std::shared_ptr<XMLNode> e_name = std::make_shared<XMLNode>(e,"NAME",e_data.getName());
                std::shared_ptr<XMLNode> e_video_path = std::make_shared<XMLNode>(e,"VIDEOPATH",e_data.getVideoPath());
                std::shared_ptr<XMLNode> e_base_vol = std::make_shared<XMLNode>(e,"BASEVOL",std::to_string(e_data.getBaseVolume()));
                std::shared_ptr<XMLNode> e_type = std::make_shared<XMLNode>(e,"TYPE",std::to_string(e_data.getExerciseType()));
                std::shared_ptr<XMLNode> e_areas = std::make_shared<XMLNode>(e,"TRAINEDAREAS","");
                for (auto m : e_data.getTrainedMuscles())
                {
                    std::shared_ptr<XMLNode> e_area = std::make_shared<XMLNode>(e_areas,"AREA",std::to_string(m));
                    e_areas->addChild(e_area);
                }
                e->addChild(e_name);
                e->addChild(e_video_path);
                e->addChild(e_base_vol);
                e->addChild(e_type);
                e->addChild(e_areas);
                root->addChild(e);
            }
        }
    }
}
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
            std::shared_ptr<XMLNode> e_thumbnail_path = std::make_shared<XMLNode>(e,"THUMBNAILPATH",ex.getThumbnailPath());
            std::shared_ptr<XMLNode> e_video_start = std::make_shared<XMLNode>(e,"VIDEOSTARTTIME",ex.getVideoStartTime());
            std::shared_ptr<XMLNode> e_video_end = std::make_shared<XMLNode>(e,"VIDEOENDTIME",ex.getVideoEndTime());
            std::shared_ptr<XMLNode> e_areas = std::make_shared<XMLNode>(e,"TRAINEDAREAS","");
            for (auto m : ex.getTrainedMuscles())
            {
                std::shared_ptr<XMLNode> e_area = std::make_shared<XMLNode>(e_areas,"AREA",std::to_string(m));
                e_areas->addChild(e_area);
            }
            e->addChild(e_name);
            e->addChild(e_video_url);
            e->addChild(e_video_path);
            e->addChild(e_thumbnail_path);
            e->addChild(e_video_start);
            e->addChild(e_video_end);
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

        std::shared_ptr<XMLNode> ProfileWriter::profileToNode(Profile p_dat)
        {
            std::shared_ptr<XMLNode> p = std::make_shared<XMLNode>(nullptr,"PROFILE","");
            std::shared_ptr<XMLNode> p_name = std::make_shared<XMLNode>(p,"NAME",p_dat.getName());
            std::shared_ptr<XMLNode> p_picture_path = std::make_shared<XMLNode>(p,"PICTUREPATH",p_dat.getPicturePath());
            std::shared_ptr<XMLNode> p_date_of_last_workout = std::make_shared<XMLNode>(p,"DATELASTWORKOUT",p_dat.getDateLastWorkout());
            std::shared_ptr<XMLNode> p_path_to_exercise_database = std::make_shared<XMLNode>(p,"PATHEXDB",p_dat.getPathToExerciseDB());
            std::shared_ptr<XMLNode> p_perf_factor = std::make_shared<XMLNode>(p,"PERFFACTOR",std::to_string(p_dat.getPerformanceFactor()));
            p->addChild(p_name);
            p->addChild(p_picture_path);
            p->addChild(p_date_of_last_workout);
            p->addChild(p_path_to_exercise_database);
            p->addChild(p_perf_factor);

            return p;
        }

        void ProfileWriter::addProfilesToNodeTree(std::list<FreeFit::Data::Profile> l_p)
        {
            for (FreeFit::Data::Profile p_data : l_p)
            {
                std::shared_ptr<XMLNode> p = profileToNode(p_data);
                root->addChild(p);
            }
        }

        void ProfileWriter::createNodeTree(std::list<FreeFit::Data::Profile> l_p)
        {
            root = std::make_shared<XMLNode>(nullptr,"PROFILES","");
            addProfilesToNodeTree(l_p);
        }
    }
}
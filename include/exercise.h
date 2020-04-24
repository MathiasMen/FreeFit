#pragma once

#include <string>
#include <set>

namespace FreeFit
{
    namespace Data
    {
        enum ExerciseType {TimeBased, RepetitionBased};

        enum MuscleGroup
            {Shoulder, UpperBack, MiddleBack,
            LowerBack, Chest, Biceps, Triceps,
            Forearms, CoreAbs, SideAbs, Glutes,
            Thigh, Harmstrings, Tibia, Calves};
        
        class Exercise
        {
            public:
                Exercise(){}
                void setName(std::string n){name = n;}
                std::string getName(){return name;}

                void setVideoPath(std::string v){video_path = v;}
                std::string getVideoPath(){return video_path;}

                void setBaseVolume(unsigned int i){base_volume = i;}
                unsigned int getBaseVolume(){return base_volume;}

                void setExerciseType(ExerciseType e_t){e_type = e_t;}
                ExerciseType getExerciseType(){return e_type;}

                void addTrainedMuscle(MuscleGroup m){trained_areas.insert(m);};
                std::set<MuscleGroup> getTrainedMuscles(){return trained_areas;}

            private:
                std::string name;
                std::string video_path;
                unsigned int base_volume;
                ExerciseType e_type;
                std::set<MuscleGroup> trained_areas;
        };
    }
}


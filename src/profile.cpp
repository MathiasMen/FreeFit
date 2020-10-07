#include "include/profile.h"

namespace FreeFit
{
    namespace Data
    {
        Profile::Profile(std::string n,std::string c,std::string p_path,std::string date,std::string db_path,double perf)
        :name(n),color(c),picture_path(p_path),date_of_last_workout(date),path_to_exercise_database(db_path),performance_factor(perf)
        {
            
        }

        bool Profile::isNewProfileProfile()
        {
            Profile p = buildNewProfileProfile();
            return (name == p.getName() &&
                    color == p.getColor() &&
                    picture_path == p.getPicturePath() &&
                    date_of_last_workout == p.getDateLastWorkout() &&
                    path_to_exercise_database == p.getPathToExerciseDB() &&
                    performance_factor == p.getPerformanceFactor());
        }
    }
}

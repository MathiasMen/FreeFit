#include "include/workoutgenerationwidget.h"

namespace FreeFit
{
    namespace GUI
    {
            WorkoutOptionBase::WorkoutOptionBase(QString text, std::shared_ptr<FreeFit::Data::WorkoutBase> w, QWidget* parent) : QRadioButton(text,parent), workout_data(w)
            {
                possible_options_widget = new QWidget(this);
                QVBoxLayout* ly = new QVBoxLayout(possible_options_widget);

                n_exercises_lbl = new QLabel(this);

                const unsigned int max_textfield_width = 300;

                number_of_rounds = new MaterialTextField("Number of rounds",possible_options_widget);
                number_of_rounds->setMaximumWidth(max_textfield_width);
                std::regex rounds_regex("[1-9]");
                auto func_rounds_regex = [rounds_regex](std::string s)->bool{return std::regex_match(s,rounds_regex);};
                number_of_rounds->setValidationFunction(func_rounds_regex);

                max_number_of_exercises = new MaterialTextField("Maximum number of exercises per round",possible_options_widget);
                max_number_of_exercises->setMaximumWidth(max_textfield_width);
                std::regex no_of_exercises_regex("[1-9]");
                auto func_no_exercises_regex = [no_of_exercises_regex](std::string s)->bool{return std::regex_match(s,no_of_exercises_regex);};
                max_number_of_exercises->setValidationFunction(func_no_exercises_regex);

                time_of_exercises = new MaterialTextField("Time of exercises",possible_options_widget);
                time_of_exercises->setMaximumWidth(max_textfield_width);
                std::regex time_of_exercise_regex("[1-9][0-9]{0,2}");
                auto func_time_of_exercise_regex = [time_of_exercise_regex](std::string s)->bool{return std::regex_match(s,time_of_exercise_regex);};
                time_of_exercises->setValidationFunction(func_time_of_exercise_regex);

                ly->addWidget(n_exercises_lbl);
                ly->addWidget(number_of_rounds);
                ly->addWidget(max_number_of_exercises);
                ly->addWidget(time_of_exercises);

                connect(number_of_rounds,&QLineEdit::textChanged,this,&WorkoutOptionBase::numberOfRoundsChanged);
                connect(max_number_of_exercises,&QLineEdit::textChanged,this,&WorkoutOptionBase::numberOfExercisesPerRoundChanged);
                connect(time_of_exercises,&QLineEdit::textChanged,this,&WorkoutOptionBase::timeOfExercisesChanged);
            }

            void WorkoutOptionBase::setPossibleExercises(std::list<FreeFit::Data::Exercise> e)
            {
                workout_data->setPossibleExercises(e);
                n_exercises_lbl->setText("Exercises in database: " + QString::number(e.size()));
            }
        
            void WorkoutOptionBase::setRounds(unsigned int r)
            {
                number_of_rounds->setText(QString::fromStdString(std::to_string(r)));
            }

            void WorkoutOptionBase::generateWorkout()
            {
                prepareWorkoutGeneration();
                workout_data->generate();
            }

            QWidget* WorkoutOptionBase::getOptionsWidget()
            {
                if (possible_options_widget != nullptr)
                    return possible_options_widget;
                else
                    return nullptr;
            }

            void WorkoutOptionBase::numberOfRoundsChanged()
            {
                if (!number_of_rounds->text().isEmpty() && number_of_rounds->validateText())
                    workout_data->setRounds(std::stoi(number_of_rounds->text().toStdString()));
            }

            void WorkoutOptionBase::numberOfExercisesPerRoundChanged()
            {
                if (!max_number_of_exercises->text().isEmpty() && max_number_of_exercises->validateText())
                    workout_data->setMaxNumberOfExercisesPerRound(std::stoi(max_number_of_exercises->text().toStdString()));
            }

            void WorkoutOptionBase::timeOfExercisesChanged()
            {
                if (!time_of_exercises->text().isEmpty() && time_of_exercises->validateText())
                    workout_data->setTimeOfExercises(std::stoi(time_of_exercises->text().toStdString()));
            }

            AllExercisesWorkoutOption::AllExercisesWorkoutOption(QString text, std::shared_ptr<FreeFit::Data::WorkoutBase> w, QWidget* parent) : WorkoutOptionBase(text,w,parent)
            {
                QSpacerItem* vertical_spacer = new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
                possible_options_widget->layout()->addItem(vertical_spacer);
            }
    
            FilteredExercisesWorkoutOption::FilteredExercisesWorkoutOption(QString text, std::shared_ptr<FreeFit::Data::FilteredByMusclesWorkout> w, QWidget* parent) : WorkoutOptionBase(text,w,parent)
            {
                muscle_areas = new HashtagBar(this);
                for (auto m : muscle_definitions.strings)
                    muscle_areas->addHashtag(m);
                possible_options_widget->layout()->addWidget(muscle_areas);
                QSpacerItem* vertical_spacer = new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
                possible_options_widget->layout()->addItem(vertical_spacer);
                specialized_workout = w;
            }

            void FilteredExercisesWorkoutOption::prepareWorkoutGeneration()
            {
                specialized_workout->setSelectedAreas(muscle_areas->getToggledStrings());
            }
    
            WorkoutGenerationWidget::WorkoutGenerationWidget(QWidget* parent) : MaterialDialog("Exercises","Workout","",parent)
            {
                connect(getAcceptButton(), &QPushButton::clicked, this, &QDialog::accept);
                connect(getRejectButton(), &QPushButton::clicked, this, &QDialog::reject);

                option_selection = new QWidget(this);
                option_selection_ly = new QVBoxLayout(option_selection);
                option_selection->setStyleSheet("background-color:#f8f8ff;");
                std::shared_ptr<FreeFit::Data::AllExercisesWorkout> w1 = std::make_shared<FreeFit::Data::AllExercisesWorkout>(std::list<FreeFit::Data::Exercise>());
                all_exercises_workout = new AllExercisesWorkoutOption("Random Exercises",w1,this);
                workout_options.push_back(all_exercises_workout);
                option_selection_ly->addWidget(all_exercises_workout);

                std::shared_ptr<FreeFit::Data::FilteredByMusclesWorkout> w2 = std::make_shared<FreeFit::Data::FilteredByMusclesWorkout>(std::list<FreeFit::Data::Exercise>());
                filtered_exercises_workout = new FilteredExercisesWorkoutOption("Filtered by muscle groups",w2,this);
                workout_options.push_back(filtered_exercises_workout);
                option_selection_ly->addWidget(filtered_exercises_workout);
                option_selection_ly->addStretch();
                addWidget(option_selection,0,0);

                all_exercises_workout->setChecked(true);

                all_exercises_workout->setRounds(3);
                filtered_exercises_workout->setRounds(3);

                options_canvas = new QStackedWidget(this);
                options_canvas->setStyleSheet("background-color:#f8f8ff;");
                options_canvas->addWidget(all_exercises_workout->getOptionsWidget());
                options_canvas->addWidget(filtered_exercises_workout->getOptionsWidget());
                addWidget(options_canvas,0,1);

                connect(all_exercises_workout,&QRadioButton::toggled,this,&WorkoutGenerationWidget::optionChanged);
                connect(filtered_exercises_workout,&QRadioButton::toggled,this,&WorkoutGenerationWidget::optionChanged);
            }

            void WorkoutGenerationWidget::setColor(std::string c)
            {
                MaterialDialog::setColor(c);
                all_exercises_workout->setColor(c);
                filtered_exercises_workout->setColor(c);
            }

            void WorkoutGenerationWidget::setPossibleExercises(std::list<FreeFit::Data::Exercise> e)
            {
                for (auto w : workout_options)
                    w->setPossibleExercises(e);
            }

            WorkoutOptionBase* WorkoutGenerationWidget::getSelectedWorkout()
            {
                for (WorkoutOptionBase* w : workout_options)
                    if (w->isChecked())
                        return w;
                return nullptr;
            }

            void WorkoutGenerationWidget::setNumberOfRounds(int n)
            {
                getSelectedWorkout()->setRounds(n);
            }

            void WorkoutGenerationWidget::accept()
            {
                getSelectedWorkout()->generateWorkout();
                QDialog::accept();
            }

            void WorkoutGenerationWidget::optionChanged()
            {
                for (unsigned int i = 0; i < workout_options.size(); i++)
                    if (workout_options[i]->isChecked())
                        options_canvas->setCurrentIndex(i);
            }
    }
}
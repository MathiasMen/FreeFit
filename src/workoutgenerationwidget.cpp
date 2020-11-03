#include "include/workoutgenerationwidget.h"

namespace FreeFit
{
    namespace GUI
    {
        ToggleContainer::ToggleContainer(const QString & title, const int a_d, QWidget *parent) : QWidget(parent), animation_duration(a_d)
        {
            toggle_button.setStyleSheet("QToolButton { border: none; }");
            toggle_button.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            toggle_button.setArrowType(Qt::ArrowType::RightArrow);
            toggle_button.setText(title);
            toggle_button.setCheckable(true);
            toggle_button.setChecked(false);

            toggle_animation = new QParallelAnimationGroup;

            header_line.setFrameShape(QFrame::HLine);
            header_line.setFrameShadow(QFrame::Sunken);
            header_line.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

            main_layout.setVerticalSpacing(0);
            main_layout.setContentsMargins(0, 0, 0, 0);
            int row = 0;
            main_layout.addWidget(&toggle_button, row, 0, 1, 1, Qt::AlignLeft);
            main_layout.addWidget(&header_line, row++, 2, 1, 1);
            setLayout(&main_layout);
        }

        void ToggleContainer::setContent(WorkoutOptionBase* c)
        {
            content = c;
            content->setMaximumHeight(0);
            content->setMinimumHeight(0);
            content->setName(toggle_button.text());

            min_height_animation = new QPropertyAnimation(content,"minimumHeight");
            max_height_animation = new QPropertyAnimation(content,"maximumHeight");
            toggle_animation->addAnimation(min_height_animation);
            toggle_animation->addAnimation(max_height_animation);

            main_layout.addWidget(content, 1, 0, 1, 3);

            QObject::connect(&toggle_button, &QToolButton::clicked, [this](){emit selected(this,!this->content->isSelected());});
            content->show();
        }

        void ToggleContainer::setSelected(bool s, bool animation)
        {
            toggle_button.setChecked(s);
            content->setSelected(s);
            updateAnimationProperties(animation);
            toggle_button.setArrowType(s ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
            toggle_animation->setDirection(s ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
            toggle_animation->start();
        }

        bool ToggleContainer::isSelected()
        {
            return content->isSelected();
        }

        void ToggleContainer::updateAnimationProperties(bool animation)
        {
            const unsigned int min_value = 0;
            const unsigned int max_value = content->sizeHint().height();

            if (animation)
            {
                min_height_animation->setDuration(animation_duration);
                max_height_animation->setDuration(animation_duration);
            }
            else
            {
                min_height_animation->setDuration(0);
                max_height_animation->setDuration(0);
            }
            min_height_animation->setStartValue(min_value);
            min_height_animation->setEndValue(max_value);
            max_height_animation->setStartValue(min_value);
            max_height_animation->setEndValue(max_value);
        }

        WorkoutOptionBase::WorkoutOptionBase(std::shared_ptr<FreeFit::Data::WorkoutBase> w, QWidget* parent) : QWidget(parent), workout_data(w)
        {
            QVBoxLayout* ly = new QVBoxLayout(this);

            n_exercises_lbl = new QLabel(this);

            const unsigned int textfield_width = 300;

            number_of_rounds = new MaterialTextField("Number of rounds",this);
            number_of_rounds->setFixedWidth(textfield_width);
            std::regex rounds_regex("[1-9]");
            auto func_rounds_regex = [rounds_regex](std::string s)->bool{return std::regex_match(s,rounds_regex);};
            number_of_rounds->setValidationFunction(func_rounds_regex);

            max_number_of_exercises = new MaterialTextField("Maximum number of exercises per round",this);
            max_number_of_exercises->setFixedWidth(textfield_width);
            std::regex no_of_exercises_regex("[1-9]");
            auto func_no_exercises_regex = [no_of_exercises_regex](std::string s)->bool{return std::regex_match(s,no_of_exercises_regex);};
            max_number_of_exercises->setValidationFunction(func_no_exercises_regex);

            time_of_exercises = new MaterialTextField("Time of exercises",this);
            time_of_exercises->setFixedWidth(textfield_width);
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

        AllExercisesWorkoutOption::AllExercisesWorkoutOption(std::shared_ptr<FreeFit::Data::WorkoutBase> w, QWidget* parent) : WorkoutOptionBase(w,parent)
        {
            QSpacerItem* vertical_spacer = new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
            layout()->addItem(vertical_spacer);
        }

        FilteredExercisesWorkoutOption::FilteredExercisesWorkoutOption(std::shared_ptr<FreeFit::Data::FilteredByMusclesWorkout> w, QWidget* parent) : WorkoutOptionBase(w,parent)
        {
            muscle_areas = new HashtagBar(this);
            for (auto m : muscle_definitions.strings)
                muscle_areas->addHashtag(m);
            layout()->addWidget(muscle_areas);
            QSpacerItem* vertical_spacer = new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
            layout()->addItem(vertical_spacer);
            specialized_workout = w;
        }

        void FilteredExercisesWorkoutOption::prepareWorkoutGeneration()
        {
            specialized_workout->setSelectedAreas(muscle_areas->getToggledStrings());
        }

        CustomExercisesWorkoutOption::CustomExercisesWorkoutOption(std::shared_ptr<FreeFit::Data::CustomExercisesWorkout> w, QWidget* parent) : WorkoutOptionBase(w,parent)
        {
            QWidget* workout_selection = new QWidget(this);
            QGridLayout* workout_selection_ly = new QGridLayout(workout_selection);
            custom_workout_selection = new QComboBox(workout_selection);
            custom_workout_selection->setEditable(true);
            save_workout_button = new QPushButton("Save",workout_selection);
            delete_workout_button = new QPushButton("Delete",workout_selection);
            QGridLayout* selection_button_ly = new QGridLayout;
            workout_selection_ly->addWidget(custom_workout_selection,0,0);
            selection_button_ly->addWidget(save_workout_button,0,0);
            selection_button_ly->addWidget(delete_workout_button,0,1);
            workout_selection_ly->addLayout(selection_button_ly,0,1);
            connect(custom_workout_selection,SIGNAL(currentIndexChanged(int)),this,SLOT(selectSavedWorkout(int)));
            connect(custom_workout_selection,SIGNAL(currentTextChanged(QString)),this,SLOT(changeCurrentWorkoutName(QString)));

            filter_container = new QWidget(this);
            filter_container_ly = new QHBoxLayout(filter_container);
            exercises_filter_ln = new MaterialTextField("Filter",filter_container);
            connect(exercises_filter_ln,&QLineEdit::textEdited,this,&CustomExercisesWorkoutOption::updateExistingExercises);

            no_of_exercises_indicator = new QLabel("",filter_container);

            filter_container_ly->addWidget(exercises_filter_ln);
            filter_container_ly->addWidget(no_of_exercises_indicator);

            lists_container = new QWidget(this);
            lists_container_ly = new QGridLayout(lists_container);

            existing_exercises_list = new QListWidget(lists_container);
            add_button = new QPushButton("Add selected",lists_container);
            remove_button = new QPushButton("Remove selected",lists_container);
            selected_exercises_list = new QListWidget(lists_container);

            existing_exercises_list->setSelectionMode(QAbstractItemView::MultiSelection);
            selected_exercises_list->setSelectionMode(QAbstractItemView::MultiSelection);

            connect(add_button,&QPushButton::clicked,this,&CustomExercisesWorkoutOption::addButtonClicked);
            connect(remove_button,&QPushButton::clicked,this,&CustomExercisesWorkoutOption::removeButtonClicked);

            lists_container_ly->addWidget(existing_exercises_list,0,0,2,1);
            lists_container_ly->addWidget(add_button,0,1,1,1);
            lists_container_ly->addWidget(remove_button,1,1,1,1);
            lists_container_ly->addWidget(selected_exercises_list,0,2,2,1);

            layout()->addWidget(workout_selection);
            layout()->addWidget(filter_container);
            layout()->addWidget(lists_container);

            QSpacerItem* vertical_spacer = new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
            layout()->addItem(vertical_spacer);
        }

        void CustomExercisesWorkoutOption::setPossibleExercises(std::list<FreeFit::Data::Exercise> e)
        {
            WorkoutOptionBase::setPossibleExercises(e);
        }

        void CustomExercisesWorkoutOption::updateExistingExercises()
        {
            FreeFit::Data::CustomExercisesWorkout current_workout = saved_workouts[custom_workout_selection->currentIndex()];
            std::list<std::string> exercise_names = current_workout.getExercisesPerRoundNames();
            std::list<std::string> filtered_exercise_names;
            
            std::list<std::string> selected_exercise_names;
            for (unsigned int i = 0; i < selected_exercises_list->count(); i++)
                selected_exercise_names.push_back(selected_exercises_list->item(i)->text().toStdString());

            std::string filter_string = exercises_filter_ln->text().toStdString();

            for (auto e : exercise_names)
            {
                std::string filter_string = exercises_filter_ln->getText().toStdString();
                bool filter_string_found;
                if (filter_string.empty())
                    filter_string_found = true;
                else
                    filter_string_found = (e.find(filter_string) != std::string::npos);
                
                bool exercise_not_selected = true;
                for (auto selected_e_name : selected_exercise_names)
                    if (selected_e_name == e)
                    {
                        exercise_not_selected = false;
                        break;
                    }

                if (filter_string_found && exercise_not_selected)
                    filtered_exercise_names.push_back(e);     
            }

            existing_exercises_list->clear();

            for (auto e : filtered_exercise_names)
                existing_exercises_list->addItem(QString::fromStdString(e));

            updateFilterLabel();
        }

        void CustomExercisesWorkoutOption::addButtonClicked()
        {
            QList<QListWidgetItem*> selected_items = existing_exercises_list->selectedItems();
            for (QListWidgetItem* s : selected_items)
            {
                QString exercise_name = s->text();
                selected_exercises_list->addItem(exercise_name);
                saved_workouts[custom_workout_selection->currentIndex()].addExerciseToRoundIfPossible(exercise_name.toStdString());
                delete s;
            }
        }

        void CustomExercisesWorkoutOption::removeButtonClicked()
        {
            QList<QListWidgetItem*> to_be_removed_items = selected_exercises_list->selectedItems();
            for (QListWidgetItem* s : to_be_removed_items)
            {
                QString exercise_name = s->text();
                existing_exercises_list->addItem(exercise_name);
                saved_workouts[custom_workout_selection->currentIndex()].removeExerciseFromRoundIfPossible(exercise_name.toStdString());
                delete s;
            }
        }

        void CustomExercisesWorkoutOption::updateFilterLabel()
        {
            unsigned int n_total_exercises = workout_data->getPossibleExercises().size();
            unsigned int n_matching_exercises = existing_exercises_list->count() + selected_exercises_list->count();
            no_of_exercises_indicator->setText(QString::number(n_matching_exercises) + " / " + QString::number(n_total_exercises));
        }

        void CustomExercisesWorkoutOption::prepareWorkoutGeneration()
        {
            for (unsigned int i = 0; i < existing_exercises_list->count(); i++)
            {
                QListWidgetItem* it = existing_exercises_list->item(i);
                workout_data->removeExerciseByName(it->text().toStdString());
            }
        }

        void CustomExercisesWorkoutOption::setColor(std::string c)
        {

        }

        void CustomExercisesWorkoutOption::selectSavedWorkout(int id_w)
        {
            existing_exercises_list->clear();
            selected_exercises_list->clear();

            FreeFit::Data::CustomExercisesWorkout w = saved_workouts[id_w];

            setPossibleExercises(w.getPossibleExercisesMinusRoundExercises());
        }

        void CustomExercisesWorkoutOption::changeCurrentWorkoutName(QString n)
        {
            FreeFit::Data::CustomExercisesWorkout* w = &saved_workouts[custom_workout_selection->currentIndex()];
            w->setName(n.toStdString());
        }

        void CustomExercisesWorkoutOption::updateSavedWorkouts()
        {
            custom_workout_selection->clear();
            for (auto w : saved_workouts)
                registerCustomWorkout(w);
        }

        void CustomExercisesWorkoutOption::registerCustomWorkout(FreeFit::Data::CustomExercisesWorkout w)
        {
            custom_workout_selection->addItem(QString::fromStdString(w.getName()));
        }

        void CustomExercisesWorkoutOption::writeXML()
        {
            std::list<FreeFit::Data::CustomExercisesWorkout> lst;
            FreeFit::Data::WorkoutWriter writer(path_to_saved_workouts);
            for (auto w : saved_workouts)
                lst.push_back(w);
            writer.createNodeTree(lst);
            writer.write();
        }

        WorkoutGenerationWidget::WorkoutGenerationWidget(QWidget* parent) : MaterialDialog("Exercises","Workout","",parent)
        {
            connect(getAcceptButton(), &QPushButton::clicked, this, &QDialog::accept);
            connect(getRejectButton(), &QPushButton::clicked, this, &QDialog::reject);

            std::shared_ptr<FreeFit::Data::AllExercisesWorkout> w1 = std::make_shared<FreeFit::Data::AllExercisesWorkout>(std::list<FreeFit::Data::Exercise>());
            ToggleContainer* all_exercises_container = new ToggleContainer("Random Exercises",300,this);
            workout_containers.push_back(all_exercises_container);
            connect(all_exercises_container,SIGNAL(selected(ToggleContainer*,bool)),this,SLOT(handleToggleContainerSelected(ToggleContainer*,bool)));
            all_exercises_workout = new AllExercisesWorkoutOption(w1);
            workout_options.push_back(all_exercises_workout);
            all_exercises_container->setContent(all_exercises_workout);
            addWidget(all_exercises_container,0,0);

            std::shared_ptr<FreeFit::Data::FilteredByMusclesWorkout> w2 = std::make_shared<FreeFit::Data::FilteredByMusclesWorkout>(std::list<FreeFit::Data::Exercise>());
            ToggleContainer* filtered_exercises_container = new ToggleContainer("Filtered by muscle groups",300,this);
            connect(filtered_exercises_container,SIGNAL(selected(ToggleContainer*,bool)),this,SLOT(handleToggleContainerSelected(ToggleContainer*,bool)));
            workout_containers.push_back(filtered_exercises_container);
            filtered_exercises_workout = new FilteredExercisesWorkoutOption(w2);
            workout_options.push_back(filtered_exercises_workout);
            filtered_exercises_container->setContent(filtered_exercises_workout);
            addWidget(filtered_exercises_container,1,0);

            std::shared_ptr<FreeFit::Data::CustomExercisesWorkout> w3 = std::make_shared<FreeFit::Data::CustomExercisesWorkout>(std::list<FreeFit::Data::Exercise>());
            ToggleContainer* custom_exercises_container = new ToggleContainer("Custom Exercises",300,this);
            connect(custom_exercises_container,SIGNAL(selected(ToggleContainer*,bool)),this,SLOT(handleToggleContainerSelected(ToggleContainer*,bool)));
            workout_containers.push_back(custom_exercises_container);
            custom_exercises_workout = new CustomExercisesWorkoutOption(w3);                
            workout_options.push_back(custom_exercises_workout);
            custom_exercises_container->setContent(custom_exercises_workout);
            addWidget(custom_exercises_container,2,0);
            
            all_exercises_workout->setRounds(3);
            filtered_exercises_workout->setRounds(3);
            custom_exercises_workout->setRounds(3);
            all_exercises_container->setSelected(true);
        }

        void WorkoutGenerationWidget::setColor(std::string c)
        {
            MaterialDialog::setColor(c);
            all_exercises_workout->setColor(c);
            filtered_exercises_workout->setColor(c);
            custom_exercises_workout->setColor(c);
        }

        void WorkoutGenerationWidget::setPossibleExercises(std::list<FreeFit::Data::Exercise> e)
        {
            for (auto w : workout_options)
                w->setPossibleExercises(e);
        }

        WorkoutOptionBase* WorkoutGenerationWidget::getSelectedWorkout()
        {
            for (WorkoutOptionBase* w : workout_options)
                if (w->isSelected())
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
            CustomExercisesWorkoutOption* c = dynamic_cast<CustomExercisesWorkoutOption*>(custom_exercises_workout);
            c->writeXML();
            QDialog::accept();
        }

        void WorkoutGenerationWidget::handleToggleContainerSelected(ToggleContainer* sender, bool selected)
        {
            if (selected)
            {
                for (auto c : workout_containers)
                    c->setSelected(false,false);

                sender->setSelected(true);
            }
        }

        void WorkoutGenerationWidget::updateCustomWorkouts()
        {
            FreeFit::Data::WorkoutXMLReader r(path_to_saved_workouts);
            CustomExercisesWorkoutOption* c_opt = dynamic_cast<CustomExercisesWorkoutOption*>(custom_exercises_workout);
            c_opt->setSavedWorkouts(r.getWorkoutList());
        }
    }
}
#include "SettingsDialog.hpp"

namespace wf
{
    SettingsDialog::SettingsDialog(
        Settings* a_settings,
        Wordfeud* a_wordfeud,
        QWidget* a_parent)
        : QDialog(a_parent, QDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint)
        , wordfeud(a_wordfeud)
        , settings(a_settings)
    {
        setModal(true);
        setLayout(&grid_layout);
        setWindowTitle("Settings");

        grid_layout.addLayout(&left_column_layout, 0, 0);
        grid_layout.addLayout(&right_column_layout, 0, 1);

        // Left collumn
        int layout_row = 0;

        createGeneralSettingsGroup(&left_column_layout, layout_row++);
        createStartupGroup(&left_column_layout, layout_row++);
        createAISettingsGroup(&left_column_layout, layout_row++);

        left_column_padding.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        left_column_layout.addWidget(&left_column_padding, layout_row++, 0);

        // Right collumn
        layout_row = 0;

        createLeftPlayerSettingsGroup(&right_column_layout, layout_row++);
        createRightPlayerSettingsGroup(&right_column_layout, layout_row++);

        right_column_padding.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        right_column_layout.addWidget(&right_column_padding, layout_row++, 0);

        // Bottom
        createButtons(&grid_layout, 1);

        layout()->setSizeConstraint(QLayout::SetFixedSize);
    }
    
    SettingsDialog::~SettingsDialog()
    { }
    
    QPushButton* SettingsDialog::getSaveAndStartNewGameButton()
    {
        return &save_and_start_new_game_button;
    }
    
    void SettingsDialog::open()
    {
        loadCurrentSettings();
        setNameFieldStates(0);
        updateSliders();

        QDialog::open();
        return;
    }
    
    void SettingsDialog::saveSettings()
    {
        // General
        settings->setLanguage(language_dropdown.currentText());
        settings->setModifierPattern(modifier_distribution_dropdown.currentText());
        settings->setLetterColouring(letter_colouring_dropdown.currentText());

        // Startup
        std::set<QString> languages_to_load;

        for (int index = 0; index < languages_to_load_list.count(); ++index)
        {
            auto language_item = languages_to_load_list.item(index);

            if (language_item->checkState() == Qt::Checked)
            {
                languages_to_load.insert(language_item->text());
            }
        }

        settings->setLanguagesToLoad(languages_to_load);

        // AI
        settings->setMinimumAITurnTime(minimum_ai_turn_time_slider.value());
        settings->setAILetterPlacingDelay(ai_letter_placing_delay_slider.value() * ai_letter_placing_delay_step);
        settings->setAutoRestartDelay(auto_restart_delay_slider.value());
        settings->enableAutoRestart(!(auto_restart_delay_slider.value() == maximum_auto_restart_delay));

        // Player 1
        PlayerSettings* left_player = settings->getLeftPlayer();
        left_player->setName(left_player_name_edit.text());
        left_player->setTypeWithString(left_player_type_dropdown.currentText());
        left_player->setRandomNameUse(left_player_random_name_checkbox.checkState());
        left_player->setAIDifficulty(left_player_ai_difficulty_slider.value());

        // Player 2
        PlayerSettings* right_player = settings->getRightPlayer();
        right_player->setName(right_player_name_edit.text());
        right_player->setTypeWithString(right_player_type_dropdown.currentText());
        right_player->setRandomNameUse(right_player_random_name_checkbox.checkState());
        right_player->setAIDifficulty(right_player_ai_difficulty_slider.value());

        emit settingsSaved();
        return;
    }
    
    void SettingsDialog::setNameFieldStates(int)
    {
        left_player_name_edit.setDisabled(left_player_random_name_checkbox.checkState());
        right_player_name_edit.setDisabled(right_player_random_name_checkbox.checkState());

        return;
    }
    
    void SettingsDialog::updateSliders()
    {
        int value = 0;
        int minimum_ai_difficulty = settings->getMinimumAIDifficulty();
        
        // Left player AI difficulty
        value = left_player_ai_difficulty_slider.value();
        left_player_ai_difficulty_display_label.setText(QString::number(value * 10) + "%");

        if (value < minimum_ai_difficulty)
        {
            left_player_ai_difficulty_slider.setValue(minimum_ai_difficulty);
        }

        // Right player AI difficulty
        value = right_player_ai_difficulty_slider.value();
        right_player_ai_difficulty_display_label.setText(QString::number(value * 10) + "%");

        if (value < minimum_ai_difficulty)
        {
            right_player_ai_difficulty_slider.setValue(minimum_ai_difficulty);
        }

        // Minimum AI turn time
        value = minimum_ai_turn_time_slider.value();
        minimum_ai_turn_time_display_label.setText(QString::number(value) + " seconds");

        // AI letter placing delay
        value = ai_letter_placing_delay_slider.value();
        ai_letter_placing_delay_display_label.setText(QString::number(value * ai_letter_placing_delay_step) + " ms");

        // AI mirror auto restart delay
        value = auto_restart_delay_slider.value();

        if (value == maximum_auto_restart_delay)
        {
            auto_restart_delay_display_label.setText("Auto restart disabled");
        }
        else
        {
            auto_restart_delay_display_label.setText(QString::number(value) + " seconds after game over");
        }

        return;
    }
    
    void SettingsDialog::viewSettingsHelp()
    {
        QDesktopServices::openUrl(QUrl{"https://github.com/Thomilist/wordfeud/wiki/Settings", QUrl::TolerantMode});
        return;
    }
    
    void SettingsDialog::createGeneralSettingsGroup(QGridLayout* a_grid_layout, int a_row)
    {
        int subrow = 0;

        createLanguageDropdown(subrow++);
        createModifierDistributionDropdown(subrow++);
        createLetterColouringDropdown(subrow++);

        general_settings.setLayout(&general_settings_layout);
        general_settings.setMinimumWidth(minimum_group_box_width);
        a_grid_layout->addWidget(&general_settings, a_row, 0);

        return;
    }
    
    void SettingsDialog::createLanguageDropdown(int a_row)
    {
        general_settings_layout.addWidget(&language_label, a_row, 0);
        general_settings_layout.addWidget(&language_dropdown, a_row, 1);

        if (settings->getLoadedLanguages().empty())
        {
            language_dropdown.addItem("Error: No dictionaries");
            language_dropdown.setDisabled(true);
            return;
        }

        int index = 0;

        for (auto language : settings->getLoadedLanguages())
        {
            
            language_dropdown.addItem(language.getName());

            if (language.getName() == settings->getTempLanguage()->getName())
            {
                language_dropdown.setCurrentIndex(index);
            }

            ++index;
        }

        return;
    }
    
    void SettingsDialog::createModifierDistributionDropdown(int a_row)
    {
        general_settings_layout.addWidget(&modifier_distribution_label, a_row, 0);
        general_settings_layout.addWidget(&modifier_distribution_dropdown, a_row, 1);

        int index = 0;

        for (auto modifier_distribution : settings->getModifierPattern()->getAllDistributions())
        {
            modifier_distribution_dropdown.addItem(modifier_distribution.second);

            if (modifier_distribution.second == settings->getTempModifierPattern())
            {
                modifier_distribution_dropdown.setCurrentIndex(index);
            }

            ++index;
        }

        return;
    }
    
    void SettingsDialog::createLetterColouringDropdown(int a_row)
    {
        general_settings_layout.addWidget(&letter_colouring_label, a_row, 0);
        general_settings_layout.addWidget(&letter_colouring_dropdown, a_row, 1);

        int index = 0;

        for (QString letter_colouring : {"Default", "Player colour"})
        {
            letter_colouring_dropdown.addItem(letter_colouring);

            if (letter_colouring == settings->getTempLetterColouring())
            {
                letter_colouring_dropdown.setCurrentIndex(index);
            }

            ++index;
        }

        return;
    }
    
    void SettingsDialog::createLeftPlayerSettingsGroup(QGridLayout* a_grid_layout, int a_row)
    {
        connect(&left_player_random_name_checkbox, &QCheckBox::stateChanged, this, &SettingsDialog::setNameFieldStates);
        connect(&left_player_ai_difficulty_slider, &QSlider::valueChanged, this, &SettingsDialog::updateSliders);
        
        // Left player name
        left_player_settings_layout.addWidget(&left_player_name_label, 0, 0);
        left_player_settings_layout.addWidget(&left_player_name_edit, 0, 1);
        left_player_settings_layout.addWidget(&left_player_random_name_checkbox, 1, 1);

        left_player_name_validator.setRegExp(player_name_regex);
        left_player_name_edit.setValidator(&left_player_name_validator);

        left_player_name_edit.setText(settings->getLeftPlayer()->getTempName());
        left_player_name_edit.setMaxLength(maximum_name_length);

        left_player_random_name_checkbox.setChecked(settings->getLeftPlayer()->usesRandomNameTemp());

        // Left player type
        left_player_settings_layout.addWidget(&left_player_type_label, 2, 0);
        left_player_settings_layout.addWidget(&left_player_type_dropdown, 2, 1);

        int index = 0;

        for (auto type : settings->getLeftPlayer()->getAllTypes())
        {
            left_player_type_dropdown.addItem(type.second);

            if (type.first == settings->getLeftPlayer()->getTempType())
            {
                left_player_type_dropdown.setCurrentIndex(index);
            }

            ++index;
        }

        // Left player AI difficulty
        left_player_ai_difficulty_slider.setMinimum(minimum_shown_ai_difficulty);
        left_player_ai_difficulty_slider.setMaximum(settings->getMaximumAIDifficulty());
        left_player_ai_difficulty_slider.setPageStep(1);
        left_player_ai_difficulty_slider.setOrientation(Qt::Orientation::Horizontal);
        left_player_ai_difficulty_slider.setValue(settings->getLeftPlayer()->getTempAIDifficulty());
        left_player_settings_layout.addWidget(&left_player_ai_difficulty_label, 3, 0);
        left_player_settings_layout.addWidget(&left_player_ai_difficulty_display_label, 3, 1);
        left_player_settings_layout.addWidget(&left_player_ai_difficulty_slider, 4, 1);
        left_player_ai_difficulty_display_label.setAlignment(Qt::AlignCenter);

        // Insert into dialog box
        left_player_settings.setLayout(&left_player_settings_layout);
        left_player_settings.setMinimumWidth(minimum_group_box_width);
        a_grid_layout->addWidget(&left_player_settings, a_row, 0);

        return;
    }
    
    void SettingsDialog::createRightPlayerSettingsGroup(QGridLayout* a_grid_layout, int a_row)
    {
        connect(&right_player_random_name_checkbox, &QCheckBox::stateChanged, this, &SettingsDialog::setNameFieldStates);
        connect(&right_player_ai_difficulty_slider, &QSlider::valueChanged, this, &SettingsDialog::updateSliders);
        
        // Right player name
        right_player_settings_layout.addWidget(&right_player_name_label, 0, 0);
        right_player_settings_layout.addWidget(&right_player_name_edit, 0, 1);
        right_player_settings_layout.addWidget(&right_player_random_name_checkbox, 1, 1);

        right_player_name_validator.setRegExp(player_name_regex);
        right_player_name_edit.setValidator(&right_player_name_validator);

        right_player_name_edit.setText(settings->getRightPlayer()->getTempName());
        right_player_name_edit.setMaxLength(maximum_name_length);

        right_player_random_name_checkbox.setChecked(settings->getRightPlayer()->usesRandomNameTemp());

        // Right player type
        right_player_settings_layout.addWidget(&right_player_type_label, 2, 0);
        right_player_settings_layout.addWidget(&right_player_type_dropdown, 2, 1);

        int index = 0;

        for (auto type : settings->getRightPlayer()->getAllTypes())
        {
            right_player_type_dropdown.addItem(type.second);

            if (type.first == settings->getRightPlayer()->getTempType())
            {
                right_player_type_dropdown.setCurrentIndex(index);
            }

            ++index;
        }

        // Right player AI difficulty
        right_player_ai_difficulty_slider.setMinimum(minimum_shown_ai_difficulty);
        right_player_ai_difficulty_slider.setMaximum(settings->getMaximumAIDifficulty());
        right_player_ai_difficulty_slider.setPageStep(1);
        right_player_ai_difficulty_slider.setOrientation(Qt::Orientation::Horizontal);
        right_player_ai_difficulty_slider.setValue(settings->getRightPlayer()->getTempAIDifficulty());
        right_player_settings_layout.addWidget(&right_player_ai_difficulty_label, 3, 0);
        right_player_settings_layout.addWidget(&right_player_ai_difficulty_display_label, 3, 1);
        right_player_settings_layout.addWidget(&right_player_ai_difficulty_slider, 4, 1);
        right_player_ai_difficulty_display_label.setAlignment(Qt::AlignCenter);

        // Insert into dialog box
        right_player_settings.setLayout(&right_player_settings_layout);
        right_player_settings.setMinimumWidth(minimum_group_box_width);
        a_grid_layout->addWidget(&right_player_settings, a_row, 0);

        return;
    }
    
    void SettingsDialog::createStartupGroup(QGridLayout* a_grid_layout, int a_row)
    {
        // List of languages to load
        createLanguagesToLoadList();
        languages_to_load_notice.setTextFormat(Qt::RichText);

        startup_settings_layout.addWidget(&languages_to_load_label, 0, 0);
        startup_settings_layout.addWidget(&languages_to_load_list, 1, 0);
        startup_settings_layout.addWidget(&languages_to_load_notice, 2, 0);

        // Insert into dialog box
        startup_settings.setLayout(&startup_settings_layout);
        startup_settings.setMinimumWidth(minimum_group_box_width);
        a_grid_layout->addWidget(&startup_settings, a_row, 0);

        return;
    }
    
    void SettingsDialog::createLanguagesToLoadList()
    {
        languages_to_load_list.clear();
        settings->detectLanguages();
        auto languages_to_load = settings->getLanguagesToLoadAsStrings();

        for (auto language : settings->getAvailableLanguagesAsStrings())
        {
            QListWidgetItem* language_item = new QListWidgetItem(language, &languages_to_load_list);

            if (languages_to_load.contains(language))
            {
                language_item->setCheckState(Qt::Checked);
            }
            else
            {
                language_item->setCheckState(Qt::Unchecked);
            }

            languages_to_load_list.addItem(language_item);
        }
        
        return;
    }
    
    void SettingsDialog::createAISettingsGroup(QGridLayout* a_grid_layout, int a_row)
    {
        connect(&minimum_ai_turn_time_slider, &QSlider::valueChanged, this, &SettingsDialog::updateSliders);
        connect(&ai_letter_placing_delay_slider, &QSlider::valueChanged, this, &SettingsDialog::updateSliders);
        connect(&auto_restart_delay_slider, &QSlider::valueChanged, this, &SettingsDialog::updateSliders);

        minimum_ai_turn_time_slider.setMinimum(minimum_ai_turn_time);
        minimum_ai_turn_time_slider.setMaximum(maximum_ai_turn_time);
        minimum_ai_turn_time_slider.setTickPosition(QSlider::TicksBothSides);
        minimum_ai_turn_time_slider.setTickInterval(1);
        minimum_ai_turn_time_slider.setPageStep(1);
        minimum_ai_turn_time_slider.setOrientation(Qt::Orientation::Horizontal);
        minimum_ai_turn_time_slider.setValue(settings->getTempMinimumAITurnTime());
        minimum_ai_turn_time_display_label.setAlignment(Qt::AlignCenter);

        ai_letter_placing_delay_slider.setMinimum(minimum_ai_letter_placing_delay / ai_letter_placing_delay_step);
        ai_letter_placing_delay_slider.setMaximum(maximum_ai_letter_placing_delay / ai_letter_placing_delay_step);
        ai_letter_placing_delay_slider.setTickPosition(QSlider::TicksBothSides);
        ai_letter_placing_delay_slider.setTickInterval(1);
        ai_letter_placing_delay_slider.setPageStep(2);
        ai_letter_placing_delay_slider.setOrientation(Qt::Orientation::Horizontal);
        ai_letter_placing_delay_slider.setValue(settings->getTempAILetterPlacingDelay() / ai_letter_placing_delay_step);
        ai_letter_placing_delay_display_label.setAlignment(Qt::AlignCenter);

        auto_restart_delay_slider.setMinimum(minimum_auto_restart_delay);
        auto_restart_delay_slider.setMaximum(maximum_auto_restart_delay);
        auto_restart_delay_slider.setTickPosition(QSlider::TicksBothSides);
        auto_restart_delay_slider.setTickInterval(1);
        auto_restart_delay_slider.setPageStep(5);
        auto_restart_delay_slider.setOrientation(Qt::Orientation::Horizontal);
        auto_restart_delay_slider.setValue(settings->getTempAutoRestartDelay());
        auto_restart_delay_display_label.setAlignment(Qt::AlignCenter);
        
        ai_settings_layout.addWidget(&minimum_ai_turn_time_label, 0, 0);
        ai_settings_layout.addWidget(&minimum_ai_turn_time_display_label, 0, 1);
        ai_settings_layout.addWidget(&minimum_ai_turn_time_slider, 1, 1);

        ai_settings_layout.addWidget(&ai_letter_placing_delay_label, 2, 0);
        ai_settings_layout.addWidget(&ai_letter_placing_delay_display_label, 2, 1);
        ai_settings_layout.addWidget(&ai_letter_placing_delay_slider, 3, 1);

        ai_settings_layout.addWidget(&auto_restart_delay_label, 4, 0);
        ai_settings_layout.addWidget(&auto_restart_delay_display_label, 4, 1);
        ai_settings_layout.addWidget(&auto_restart_delay_slider, 5, 1);
        
        ai_settings.setLayout(&ai_settings_layout);
        ai_settings.setMinimumWidth(minimum_group_box_width);
        a_grid_layout->addWidget(&ai_settings, a_row, 0);

        return;
    }
    
    void SettingsDialog::createButtons(QGridLayout* a_grid_layout, int a_row)
    {
        buttons.addButton(QDialogButtonBox::Help);
        buttons.addButton(&save_and_start_new_game_button, QDialogButtonBox::AcceptRole);
        buttons.addButton(QDialogButtonBox::Save);
        buttons.addButton(QDialogButtonBox::Cancel);

        for (auto button : buttons.findChildren<QAbstractButton*>())
        {
            button->setIcon(QIcon{});
        }

        connect(&buttons, &QDialogButtonBox::helpRequested, this, &SettingsDialog::viewSettingsHelp);
        connect(&buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(&buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        connect(&buttons, &QDialogButtonBox::accepted, this, &SettingsDialog::saveSettings);

        a_grid_layout->addWidget(&buttons, a_row, 0, 1, 2, Qt::AlignCenter);

        return;
    }
    
    void SettingsDialog::loadCurrentSettings()
    {
        // General
        for (int index = 0; index < language_dropdown.count() && !settings->getLoadedLanguages().empty(); ++index)
        {
            if (language_dropdown.itemText(index) == settings->getTempLanguage()->getName())
            {
                language_dropdown.setCurrentIndex(index);
                break;
            }
        }

        for (int index = 0; index < modifier_distribution_dropdown.count(); ++index)
        {
            if (modifier_distribution_dropdown.itemText(index) == settings->getTempModifierPattern())
            {
                modifier_distribution_dropdown.setCurrentIndex(index);
                break;
            }
        }

        for (int index = 0; index < letter_colouring_dropdown.count(); ++index)
        {
            if (letter_colouring_dropdown.itemText(index) == settings->getTempLetterColouring())
            {
                letter_colouring_dropdown.setCurrentIndex(index);
                break;
            }
        }

        // Startup
        createLanguagesToLoadList();

        // AI
        minimum_ai_turn_time_slider.setValue(settings->getTempMinimumAITurnTime());
        ai_letter_placing_delay_slider.setValue(settings->getTempAILetterPlacingDelay() / ai_letter_placing_delay_step);
        auto_restart_delay_slider.setValue(settings->getTempAutoRestartDelay());

        // Player 1
        left_player_name_edit.setText(settings->getLeftPlayer()->getTempName());
        left_player_random_name_checkbox.setChecked(settings->getLeftPlayer()->usesRandomNameTemp());

        for (int index = 0; index < left_player_type_dropdown.count(); ++index)
        {
            if (left_player_type_dropdown.itemText(index) == settings->getLeftPlayer()->getTempTypeAsString())
            {
                left_player_type_dropdown.setCurrentIndex(index);
                break;
            }
        }
        
        left_player_ai_difficulty_slider.setValue(settings->getLeftPlayer()->getTempAIDifficulty());

        // Player 2
        right_player_name_edit.setText(settings->getRightPlayer()->getTempName());
        right_player_random_name_checkbox.setChecked(settings->getRightPlayer()->usesRandomNameTemp());

        for (int index = 0; index < right_player_type_dropdown.count(); ++index)
        {
            if (right_player_type_dropdown.itemText(index) == settings->getRightPlayer()->getTempTypeAsString())
            {
                right_player_type_dropdown.setCurrentIndex(index);
                break;
            }
        }
        
        right_player_ai_difficulty_slider.setValue(settings->getRightPlayer()->getTempAIDifficulty());

        return;
    }
}
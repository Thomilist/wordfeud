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
        
        createGeneralSettingsGroup();
        createLeftPlayerSettingsGroup();
        createRightPlayerSettingsGroup();
        createAISettingsGroup();
        createButtons();

        setNameFieldStates(0);
        updateSliders();

        layout()->setSizeConstraint(QLayout::SetFixedSize);
    }
    
    SettingsDialog::~SettingsDialog()
    { }
    
    QPushButton* SettingsDialog::getSaveAndStartNewGameButton()
    {
        return &save_and_start_new_game_button;
    }
    
    void SettingsDialog::saveSettings()
    {
        settings->setLanguage(language_dropdown.currentText());
        settings->setModifierPattern(modifier_distribution_dropdown.currentText());
        settings->setLetterColouring(letter_colouring_dropdown.currentText());

        PlayerSettings* left_player = settings->getLeftPlayer();
        left_player->setName(left_player_name_edit.text());
        left_player->setTypeWithString(left_player_type_dropdown.currentText());
        left_player->setRandomNameUse(left_player_random_name_checkbox.checkState());
        left_player->setAIDifficulty(left_player_ai_difficulty_slider.value());

        PlayerSettings* right_player = settings->getRightPlayer();
        right_player->setName(right_player_name_edit.text());
        right_player->setTypeWithString(right_player_type_dropdown.currentText());
        right_player->setRandomNameUse(right_player_random_name_checkbox.checkState());
        right_player->setAIDifficulty(right_player_ai_difficulty_slider.value());

        settings->setMinimumAITurnTime(minimum_ai_turn_time_slider.value());
        settings->setAILetterPlacingDelay(ai_letter_placing_delay_slider.value() * ai_letter_placing_delay_step);

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

        return;
    }
    
    void SettingsDialog::createGeneralSettingsGroup()
    {
        int subrow = 0;

        createLanguageDropdown(subrow++);
        createModifierDistributionDropdown(subrow++);
        createLetterColouringDropdown(subrow++);

        general_settings.setLayout(&general_settings_layout);
        grid_layout.addWidget(&general_settings, layout_row, 0);

        ++layout_row;

        return;
    }
    
    void SettingsDialog::createLanguageDropdown(int a_row)
    {
        general_settings_layout.addWidget(&language_label, a_row, 0);
        general_settings_layout.addWidget(&language_dropdown, a_row, 1);

        int index = 0;

        for (auto language : settings->getAvailableLanguages())
        {
            language_dropdown.addItem(language.asString());

            if (language.asEnum() == settings->getTempLanguage()->asEnum())
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
    
    void SettingsDialog::createLeftPlayerSettingsGroup()
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
        grid_layout.addWidget(&left_player_settings, layout_row, 0);
        ++layout_row;

        return;
    }
    
    void SettingsDialog::createRightPlayerSettingsGroup()
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
        grid_layout.addWidget(&right_player_settings, layout_row, 0);
        ++layout_row;

        return;
    }
    
    void SettingsDialog::createAISettingsGroup()
    {
        connect(&minimum_ai_turn_time_slider, &QSlider::valueChanged, this, &SettingsDialog::updateSliders);
        connect(&ai_letter_placing_delay_slider, &QSlider::valueChanged, this, &SettingsDialog::updateSliders);

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
        
        ai_settings_layout.addWidget(&minimum_ai_turn_time_label, 0, 0);
        ai_settings_layout.addWidget(&minimum_ai_turn_time_display_label, 0, 1);
        ai_settings_layout.addWidget(&minimum_ai_turn_time_slider, 1, 1);

        ai_settings_layout.addWidget(&ai_letter_placing_delay_label, 2, 0);
        ai_settings_layout.addWidget(&ai_letter_placing_delay_display_label, 2, 1);
        ai_settings_layout.addWidget(&ai_letter_placing_delay_slider, 3, 1);
        
        ai_settings.setLayout(&ai_settings_layout);
        grid_layout.addWidget(&ai_settings, layout_row, 0);
        ++layout_row;

        return;
    }
    
    void SettingsDialog::createButtons()
    {
        grid_layout.addWidget(&buttons, layout_row++, 0, 1, 2);
        buttons.addButton(&save_and_start_new_game_button, QDialogButtonBox::AcceptRole);
        buttons.addButton(QDialogButtonBox::Save);
        buttons.addButton(QDialogButtonBox::Cancel);

        for (auto button : buttons.findChildren<QAbstractButton*>())
        {
            button->setIcon(QIcon{});
        }

        connect(&buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(&buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        connect(&buttons, &QDialogButtonBox::accepted, this, &SettingsDialog::saveSettings);

        return;
    }
}
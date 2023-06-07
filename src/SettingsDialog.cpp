#include "SettingsDialog.hpp"

namespace wf
{
    SettingsDialog::SettingsDialog(
        Settings* a_settings,
        Wordfeud* a_wordfeud,
        QWidget* a_parent)
        : QDialog(a_parent)
        , wordfeud(a_wordfeud)
        , settings(a_settings)
    {
        setModal(true);
        setLayout(&grid_layout);
        setWindowTitle("Settings");
        
        createLanguageDropdown();
        createModifierDistributionDropdown();
        createLeftPlayerSettingsGroup();
        createRightPlayerSettingsGroup();
        createButtons();

        layout()->setSizeConstraint(QLayout::SetFixedSize);
    }
    
    SettingsDialog::~SettingsDialog()
    { }
    
    QPushButton* SettingsDialog::getSaveAndStartNewGameButton()
    {
        return &save_and_start_new_game_button;
    }
    
    void SettingsDialog::createLanguageDropdown()
    {
        grid_layout.addWidget(&language_label, layout_row, 0);
        grid_layout.addWidget(&language_dropdown, layout_row, 1);
        ++layout_row;

        int index = 0;

        for (auto language : settings->getAvailableLanguages())
        {
            language_dropdown.addItem(language.asString());

            if (language.asEnum() == settings->getLanguage()->asEnum())
            {
                language_dropdown.setCurrentIndex(index);
            }

            ++index;
        }

        return;
    }
    
    void SettingsDialog::createModifierDistributionDropdown()
    {
        grid_layout.addWidget(&modifier_distribution_label, layout_row, 0);
        grid_layout.addWidget(&modifier_distribution_dropdown, layout_row, 1);
        ++layout_row;

        int index = 0;

        for (auto modifier_distribution : settings->getModifierPattern()->getAllDistributions())
        {
            modifier_distribution_dropdown.addItem(modifier_distribution.second);

            if (modifier_distribution.first == settings->getModifierPattern()->getDistribution())
            {
                modifier_distribution_dropdown.setCurrentIndex(index);
            }

            ++index;
        }

        return;
    }
    
    void SettingsDialog::createLeftPlayerSettingsGroup()
    {
        // Left player name
        left_player_settings_layout.addWidget(&left_player_name_label, 0, 0);
        left_player_settings_layout.addWidget(&left_player_name_edit, 0, 1);
        left_player_settings_layout.addWidget(&left_player_random_name_checkbox, 1, 1);

        left_player_name_validator.setRegExp(player_name_regex);
        left_player_name_edit.setValidator(&left_player_name_validator);

        left_player_name_edit.setText(settings->getLeftPlayer()->getName());
        left_player_name_edit.setMaxLength(24);
        left_player_name_edit.setFixedWidth(199);

        // Left player type
        left_player_settings_layout.addWidget(&left_player_type_label, 2, 0);
        left_player_settings_layout.addWidget(&left_player_type_dropdown, 2, 1);

        int index = 0;

        for (auto type : settings->getLeftPlayer()->getAllTypes())
        {
            left_player_type_dropdown.addItem(type.second);

            if (type.first == settings->getLeftPlayer()->getType())
            {
                left_player_type_dropdown.setCurrentIndex(index);
            }

            ++index;
        }

        // Insert into dialog box
        left_player_settings.setLayout(&left_player_settings_layout);
        grid_layout.addWidget(&left_player_settings, layout_row, 0, 1, 2);
        ++layout_row;

        return;
    }
    
    void SettingsDialog::createRightPlayerSettingsGroup()
    {
        // Right player name
        right_player_settings_layout.addWidget(&right_player_name_label, 0, 0);
        right_player_settings_layout.addWidget(&right_player_name_edit, 0, 1);
        right_player_settings_layout.addWidget(&right_player_random_name_checkbox, 1, 1);

        right_player_name_validator.setRegExp(player_name_regex);
        right_player_name_edit.setValidator(&right_player_name_validator);

        right_player_name_edit.setText(settings->getRightPlayer()->getName());
        right_player_name_edit.setMaxLength(24);
        right_player_name_edit.setFixedWidth(199);

        // Right player type
        right_player_settings_layout.addWidget(&right_player_type_label, 2, 0);
        right_player_settings_layout.addWidget(&right_player_type_dropdown, 2, 1);

        int index = 0;

        for (auto type : settings->getRightPlayer()->getAllTypes())
        {
            right_player_type_dropdown.addItem(type.second);

            if (type.first == settings->getRightPlayer()->getType())
            {
                right_player_type_dropdown.setCurrentIndex(index);
            }

            ++index;
        }

        // Insert into dialog box
        right_player_settings.setLayout(&right_player_settings_layout);
        grid_layout.addWidget(&right_player_settings, layout_row, 0, 1, 2);
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
    
    void SettingsDialog::saveSettings()
    {
        settings->setLanguage(language_dropdown.currentText());
        settings->getModifierPattern()->setDistribution(modifier_distribution_dropdown.currentText());
        
        PlayerSettings* left_player = settings->getLeftPlayer();
        PlayerSettings* right_player = settings->getRightPlayer();

        left_player->setName(left_player_name_edit.text());
        right_player->setName(right_player_name_edit.text());

        left_player->setTypeWithString(left_player_type_dropdown.currentText());
        right_player->setTypeWithString(right_player_type_dropdown.currentText());

        return;
    }
}
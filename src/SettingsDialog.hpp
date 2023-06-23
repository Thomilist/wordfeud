#ifndef __SETTINGSDIALOG_H__
#define __SETTINGSDIALOG_H__

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QRegExp>
#include <QRegExpValidator>
#include <QSizePolicy>
#include <QSlider>
#include <QString>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "Settings.hpp"

namespace wf
{
    class SettingsDialog : public QDialog
    {
        Q_OBJECT

        public:
            SettingsDialog(
                Settings* a_settings,
                Wordfeud* a_wordfeud,
                QWidget* a_parent);
            ~SettingsDialog();

            QPushButton* getSaveAndStartNewGameButton();

        public slots:
            void saveSettings();
            void setNameFieldStates(int a_state);
            void updateSliders();

        signals:
            void settingsSaved();
        
        private:
            void createGeneralSettingsGroup();
            void createLanguageDropdown(int a_row);
            void createModifierDistributionDropdown(int a_row);
            void createLetterColouringDropdown(int a_row);
            void createLeftPlayerSettingsGroup();
            void createRightPlayerSettingsGroup();
            void createAISettingsGroup();
            void createButtons();

            Wordfeud* wordfeud;
            Settings* settings;
            QGridLayout grid_layout;
            int layout_row = 0;
            QRegExp player_name_regex{QString{"^[\\w ]*[^\\W_][\\w ]*$"}};
            const int maximum_name_length = 26;
            const int minimum_shown_ai_difficulty = 0;
            const int minimum_ai_turn_time = 0;
            const int maximum_ai_turn_time = 10;
            const int minimum_ai_letter_placing_delay = 0;
            const int maximum_ai_letter_placing_delay = 1000;
            const int ai_letter_placing_delay_step = 50;

            // General settings
            QGroupBox general_settings{"General"};
            QGridLayout general_settings_layout;
            QLabel language_label{"Dictionary:"};
            QComboBox language_dropdown;
            QLabel modifier_distribution_label{"Modifier distribution:"};
            QComboBox modifier_distribution_dropdown;
            QLabel letter_colouring_label{"Letter colouring:"};
            QComboBox letter_colouring_dropdown;

            // Left player settings
            QGroupBox left_player_settings{"Player 1"};
            QGridLayout left_player_settings_layout;
            QLabel left_player_name_label{"Name:"};
            QLineEdit left_player_name_edit;
            QRegExpValidator left_player_name_validator;
            QCheckBox left_player_random_name_checkbox{"Use random name"};
            QLabel left_player_type_label{"Control:"};
            QComboBox left_player_type_dropdown;
            QLabel left_player_ai_difficulty_label{"AI difficulty:"};
            QLabel left_player_ai_difficulty_display_label;
            QSlider left_player_ai_difficulty_slider;

            // Right player settings
            QGroupBox right_player_settings{"Player 2"};
            QGridLayout right_player_settings_layout;
            QLabel right_player_name_label{"Name:"};
            QLineEdit right_player_name_edit;
            QRegExpValidator right_player_name_validator;
            QCheckBox right_player_random_name_checkbox{"Use random name"};
            QLabel right_player_type_label{"Control:"};
            QComboBox right_player_type_dropdown;
            QLabel right_player_ai_difficulty_label{"AI difficulty:"};
            QLabel right_player_ai_difficulty_display_label;
            QSlider right_player_ai_difficulty_slider;

            // AI settings
            QGroupBox ai_settings{"AI"};
            QGridLayout ai_settings_layout;
            QLabel minimum_ai_turn_time_label{"Minimum turn time:"};
            QLabel minimum_ai_turn_time_display_label;
            QSlider minimum_ai_turn_time_slider;
            QLabel ai_letter_placing_delay_label{"Letter placing delay:"};
            QLabel ai_letter_placing_delay_display_label;
            QSlider ai_letter_placing_delay_slider;

            // Custom buttons
            QDialogButtonBox buttons;
            QPushButton save_and_start_new_game_button{"Save and start new game"};
    };
}

#endif // __SETTINGSDIALOG_H__
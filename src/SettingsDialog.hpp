#ifndef __SETTINGSDIALOG_H__
#define __SETTINGSDIALOG_H__

#include <set>

#include <QDebug>

#include <QCheckBox>
#include <QComboBox>
#include <QDesktopServices>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSizePolicy>
#include <QSlider>
#include <QString>
#include <QStringBuilder>
#include <QUrl>
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
                QWidget* a_parent = nullptr);
            ~SettingsDialog();

            QPushButton* getSaveAndStartNewGameButton();

        public slots:
            void open();
            void saveSettings();
            void setNameFieldStates(int a_state);
            void updateSliders();
            void viewSettingsHelp();

        signals:
            void settingsSaved();
        
        private:
            void createGeneralSettingsGroup(QGridLayout* a_grid_layout, int a_row);
            void createLanguageDropdown(int a_row);
            void createModifierDistributionDropdown(int a_row);
            void createLetterColouringDropdown(int a_row);
            void createLeftPlayerSettingsGroup(QGridLayout* a_grid_layout, int a_row);
            void createRightPlayerSettingsGroup(QGridLayout* a_grid_layout, int a_row);
            void createStartupGroup(QGridLayout* a_grid_layout, int a_row);
            void createLanguagesToLoadList();
            void createAISettingsGroup(QGridLayout* a_grid_layout, int a_row);
            void createButtons(QGridLayout* a_grid_layout, int a_row);
            void loadCurrentSettings();

            Wordfeud* wordfeud;
            Settings* settings;
            QGridLayout grid_layout;
            QGridLayout left_column_layout;
            QGridLayout right_column_layout;
            QWidget left_column_padding;
            QWidget right_column_padding;
            QRegularExpression player_name_regex{QString{"^[\\w ]*[^\\W_][\\w ]*$"}};
            const int minimum_group_box_width = 400;
            const int maximum_name_length = 26;
            const int minimum_shown_ai_difficulty = 0;
            const int minimum_ai_turn_time = 0;
            const int maximum_ai_turn_time = 10;
            const int minimum_ai_letter_placing_delay = 0;
            const int maximum_ai_letter_placing_delay = 1000;
            const int ai_letter_placing_delay_step = 50;
            const int minimum_auto_restart_delay = 0;
            const int maximum_auto_restart_delay = 31;
            const int minimum_bias_strength = 0;
            const int maximum_bias_strength = 20;
            const int bias_strength_step = 1;

            // General settings
            QGroupBox general_settings{"General"};
            QGridLayout general_settings_layout;
            QLabel language_label{"Dictionary:"};
            QComboBox language_dropdown;
            QLabel modifier_distribution_label{"Board:"};
            QComboBox modifier_distribution_dropdown;
            QCheckBox shuffle_modifiers_checkbox{"Shuffle modifiers"};
            QLabel letter_colouring_label{"Letter colouring:"};
            QComboBox letter_colouring_dropdown;

            // Left player settings
            QGroupBox left_player_settings{"Player 1"};
            QGridLayout left_player_settings_layout;
            QLabel left_player_name_label{"Name:"};
            QLineEdit left_player_name_edit;
            QRegularExpressionValidator left_player_name_validator;
            QCheckBox left_player_random_name_checkbox{"Use random name"};
            QLabel left_player_type_label{"Control:"};
            QComboBox left_player_type_dropdown;
            QLabel left_player_ai_difficulty_label{"AI difficulty:"};
            QLabel left_player_ai_difficulty_display_label;
            QSlider left_player_ai_difficulty_slider;

            // Startup group
            QGroupBox startup_settings{"Startup"};
            QGridLayout startup_settings_layout;
            QLabel languages_to_load_label{"Dictionaries to load:"};
            QListWidget languages_to_load_list;
            QLabel languages_to_load_notice{"<i>These settings take effect at program startup</i>"};

            // Right player settings
            QGroupBox right_player_settings{"Player 2"};
            QGridLayout right_player_settings_layout;
            QLabel right_player_name_label{"Name:"};
            QLineEdit right_player_name_edit;
            QRegularExpressionValidator right_player_name_validator;
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
            QLabel auto_restart_delay_label{"Auto restart AI mirrors:"};
            QLabel auto_restart_delay_display_label;
            QSlider auto_restart_delay_slider;
            QLabel bias_strength_label{"Biased word strength:"};
            QLabel bias_strength_display_label;
            QSlider bias_strength_slider;

            // Custom buttons
            QDialogButtonBox buttons;
            QPushButton save_and_start_new_game_button{"Save and start new game"};
    };
}

#endif // __SETTINGSDIALOG_H__
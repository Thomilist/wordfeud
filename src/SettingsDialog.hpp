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
        
        private:
            void createLanguageDropdown();
            void createModifierDistributionDropdown();
            void createLeftPlayerSettingsGroup();
            void createRightPlayerSettingsGroup();
            void createButtons();

            Wordfeud* wordfeud;
            Settings* settings;
            QGridLayout grid_layout;
            int layout_row = 0;
            const int max_name_length = 26;
            const int name_edit_width = 199;

            QLabel language_label{"Dictionary:"};
            QComboBox language_dropdown;

            QLabel modifier_distribution_label{"Modifier distribution:"};
            QComboBox modifier_distribution_dropdown;

            QRegExp player_name_regex{QString{"^[\\w ]*[^\\W_][\\w ]*$"}};

            QGroupBox left_player_settings{"Player 1"};
            QGridLayout left_player_settings_layout;
            QLabel left_player_name_label{"Name:"};
            QLineEdit left_player_name_edit;
            QRegExpValidator left_player_name_validator;
            QCheckBox left_player_random_name_checkbox{"Use random name"};
            QLabel left_player_type_label{"Control:"};
            QComboBox left_player_type_dropdown;

            QGroupBox right_player_settings{"Player 2"};
            QGridLayout right_player_settings_layout;
            QLabel right_player_name_label{"Name:"};
            QLineEdit right_player_name_edit;
            QRegExpValidator right_player_name_validator;
            QCheckBox right_player_random_name_checkbox{"Use random name"};
            QLabel right_player_type_label{"Control:"};
            QComboBox right_player_type_dropdown;

            QDialogButtonBox buttons;
            QPushButton save_and_start_new_game_button{"Save and start new game"};
    };
}

#endif // __SETTINGSDIALOG_H__
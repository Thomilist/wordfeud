#ifndef __SETTINGSDIALOG_H__
#define __SETTINGSDIALOG_H__

#include <QDialog>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QIcon>

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
        
        private:
            void createLanguageDropdown();
            void createModifierDistributionDropdown();
            void createButtons();

            Wordfeud* wordfeud;
            Settings* settings;
            QGridLayout layout;
            int layout_row = 0;

            QLabel language_label{"Dictionary:"};
            QComboBox language_dropdown;

            QLabel modifier_distribution_label{"Modifier distribution:"};
            QComboBox modifier_distribution_dropdown;

            QDialogButtonBox buttons;
            QPushButton save_and_start_new_game_button{"Save and start new game"};
    };
}

#endif // __SETTINGSDIALOG_H__
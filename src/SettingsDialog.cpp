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
        setLayout(&layout);
        
        createLanguageDropdown();
        createModifierDistributionDropdown();
        createButtons();
    }
    
    SettingsDialog::~SettingsDialog()
    { }
    
    QPushButton* SettingsDialog::getSaveAndStartNewGameButton()
    {
        return &save_and_start_new_game_button;
    }
    
    void SettingsDialog::createLanguageDropdown()
    {
        layout.addWidget(&language_label, layout_row, 0);
        layout.addWidget(&language_dropdown, layout_row, 1);
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
        layout.addWidget(&modifier_distribution_label, layout_row, 0);
        layout.addWidget(&modifier_distribution_dropdown, layout_row, 1);
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
    
    void SettingsDialog::createButtons()
    {
        layout.addWidget(&buttons, layout_row++, 0, 1, 2);
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
        return;
    }
}
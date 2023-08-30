#include "StartupProgressDialog.hpp"


namespace wf
{
    StartupProgressDialog::StartupProgressDialog(QWidget* a_parent)
    : dialog(a_parent, Qt::FramelessWindowHint)
    {
        dialog.setWindowIcon(QIcon{":/icon/icon.ico"});
        dialog.setMinimumWidth(500);
        language_count = std::numeric_limits<int>::max() - the_rest_count;
        updateMaximum();
        dialog.setMinimum(progress);
        dialog.setMinimumDuration(0);
        dialog.setCancelButton(nullptr);
        dialog.open();
    }

    StartupProgressDialog::~StartupProgressDialog()
    { }

    void StartupProgressDialog::incrementProgress()
    {
        dialog.setValue(++progress);
        return;
    }

    void StartupProgressDialog::setLanguageCount(int a_count)
    {
        language_count = a_count;
        updateMaximum();
        return;
    }

    void StartupProgressDialog::setText(QString a_text)
    {
        dialog.setLabelText("Starting wordfeud\n" + a_text);
        dialog.repaint();
        return;
    }

    void StartupProgressDialog::updateMaximum()
    {
        dialog.setMaximum(language_count + modifier_pattern_count + name_count + other_settings_count + the_rest_count);
        return;
    }
}

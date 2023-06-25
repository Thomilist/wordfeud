#include "UpdateDialog.hpp"


namespace wf
{
    UpdateDialog::UpdateDialog(Version* a_version, QWidget* a_parent)
        : QDialog(a_parent, QDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint)
        , version(a_version)
        , buttons(QDialogButtonBox::Ok)
    {
        setModal(true);
        setWindowTitle("Check for updates");
        connect(&buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);

        update_text.setTextFormat(Qt::RichText);
        update_text.setTextInteractionFlags(Qt::TextBrowserInteraction);
        update_text.setOpenExternalLinks(true);
        update_text.setAlignment(Qt::AlignCenter);

        grid_layout.addWidget(&update_text, 0, 0, Qt::AlignCenter);
        grid_layout.addWidget(&buttons, 1, 0, Qt::AlignCenter);
        setLayout(&grid_layout);
    }
    
    UpdateDialog::~UpdateDialog()
    { }
    
    void UpdateDialog::manualUpdateCheck()
    {
        open(version->isUpdateAvailable(), false);
        return;
    }
    
    void UpdateDialog::open(bool a_update_available, bool a_quiet)
    {
        if (a_update_available)
        {
            createNewVersionAvailable();
        }
        else if (a_quiet)
        {
            return;
        }
        else
        {
            createYouHaveLatestVersion();
        }

        QDialog::open();
        return;
    }
    
    void UpdateDialog::createYouHaveLatestVersion()
    {
        update_text.setText(
            QString()
            + "You have the latest version ("
            + version->getCurrentVersion()
            + ")."
        );

        return;
    }
    
    void UpdateDialog::createNewVersionAvailable()
    {
        update_text.setText(
            QString()
            + "There is a newer version available online.<br>"
            + "You have version "
            + version->getCurrentVersion()
            + ", but version "
            + version->getUpstreamVersion()
            + " is available for download "
            + "<a href=\"https://github.com/Thomilist/wordfeud/releases/latest/\">here</a>."
        );

        return;
    }
}
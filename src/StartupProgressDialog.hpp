#ifndef __STARTUPPROGRESSDIALOG_H__
#define __STARTUPPROGRESSDIALOG_H__

#include <limits>

#include <QIcon>
#include <QProgressDialog>
#include <QWidget>

#include "ForwardDeclarations.hpp"

namespace wf
{
    class StartupProgressDialog : public QObject
    {
        Q_OBJECT

        public:
            StartupProgressDialog(QWidget* a_parent = nullptr);
            ~StartupProgressDialog();

        public slots:
            void incrementProgress();
            void setLanguageCount(int a_count);
            void setText(QString a_text);

        private:
            void updateMaximum();

            QProgressDialog dialog;
            
            int progress = 0;
            int language_count;
            int name_count = 1;
            int other_settings_count = 1;
            int the_rest_count = 1;
    };
}

#endif // __STARTUPPROGRESSDIALOG_H__
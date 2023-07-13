#ifndef __LETTEREDITORWIDGET_H__
#define __LETTEREDITORWIDGET_H__

#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "Letter.hpp"
#include "RenderedTile.hpp"

#include "BoardType.hpp"

namespace wf
{
    class LetterEditorWidget : public QWidget
    {
        Q_OBJECT

        public:
            LetterEditorWidget(Settings* a_settings, QWidget* a_parent = nullptr);
            ~LetterEditorWidget();
        
        public slots:
            void updateLetterText(QString a_text);
            void updateLetterCount(int a_count);
            void updateLetterPoints(int a_points);

        signals:
            void addToPool(LetterData a_letter_data);
            void removeSelected();
        
        private:
            Settings* settings;
            QGridLayout grid_layout;
            QFormLayout form_layout;

            LetterData letter_data;
            Letter letter;

            const int edit_width = 80;
            const int button_width = 150;
            const int maximum_text_length = 3;
            const int minimum_count = 0;
            const int maximum_count = 99;
            const int minimum_points = 0;
            const int maximum_points = 99;

            QLineEdit letter_text_edit;
            QSpinBox letter_count_edit;
            QSpinBox letter_points_edit;
            RenderedTile letter_display;
            QPushButton add_letter_button{"Add to Pool"};
            QPushButton remove_selected_button{"Removed Selected"};
    };
}

#endif // __LETTEREDITORWIDGET_H__
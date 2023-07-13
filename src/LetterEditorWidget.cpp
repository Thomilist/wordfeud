#include "LetterEditorWidget.hpp"


namespace wf
{
    LetterEditorWidget::LetterEditorWidget(Settings* a_settings, QWidget* a_parent)
        : QWidget(a_parent)
        , settings(a_settings)
        , letter_display(settings, nullptr, BoardType::Display, this)
    {
        letter_display.setDisabled(true);
        letter_display.overwriteLetter(&letter);

        connect(&letter_text_edit, &QLineEdit::textChanged, this, &LetterEditorWidget::updateLetterText);
        connect(&letter_count_edit, QOverload<int>::of(&QSpinBox::valueChanged), this, &LetterEditorWidget::updateLetterCount);
        connect(&letter_points_edit, QOverload<int>::of(&QSpinBox::valueChanged), this, &LetterEditorWidget::updateLetterPoints);
        connect(&add_letter_button, &QPushButton::clicked, this, [this](){emit addToPool(letter_data);});
        connect(&remove_selected_button, &QPushButton::clicked, this, &LetterEditorWidget::removeSelected);

        letter_text_edit.setMaxLength(maximum_text_length);
        letter_count_edit.setMinimum(minimum_count);
        letter_count_edit.setMaximum(maximum_count);
        letter_points_edit.setMinimum(minimum_points);
        letter_points_edit.setMaximum(maximum_points);

        letter_text_edit.setMaximumWidth(edit_width);
        add_letter_button.setFixedWidth(button_width);
        remove_selected_button.setFixedWidth(button_width);

        form_layout.addRow("Text:", &letter_text_edit);
        form_layout.addRow("Count:", &letter_count_edit);
        form_layout.addRow("Points:", &letter_points_edit);

        grid_layout.addLayout(&form_layout, 0, 0, Qt::AlignCenter);
        grid_layout.addWidget(&letter_display, 0, 1, 3, 1, Qt::AlignCenter);
        grid_layout.addWidget(&add_letter_button, 1, 0, Qt::AlignHCenter | Qt::AlignBottom);
        grid_layout.addWidget(&remove_selected_button, 2, 0, Qt::AlignHCenter | Qt::AlignTop);

        setLayout(&grid_layout);
    }
    
    LetterEditorWidget::~LetterEditorWidget()
    { }
    
    void LetterEditorWidget::updateLetterText(QString a_text)
    {
        letter_data.letter = a_text;
        letter.setText(a_text);
        letter_display.repaint();
        return;
    }
    
    void LetterEditorWidget::updateLetterCount(int a_count)
    {
        letter_data.count = a_count;
        letter_display.repaint();
        return;
    }
    
    void LetterEditorWidget::updateLetterPoints(int a_points)
    {
        letter_data.points = a_points;
        letter.setPoints(a_points);
        letter_display.repaint();
        return;
    }
}
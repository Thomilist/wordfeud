#include "ButtonPanel.hpp"

namespace wf
{
    ButtonPanel::ButtonPanel(QSize a_size, QWidget* a_parent)
        : QWidget(a_parent)
        , size(a_size)
        , button_width(a_size.width() / 4)
        , play_pass_stack(this)
        , clear_shuffle_confirm_stack(this)
        , swap_cancel_stack(this)
        , play_button("Play", this)
        , pass_button("Pass", this)
        , clear_button("Clear", this)
        , shuffle_button("Shuffle", this)
        , swap_button("Swap", this)
        , confirm_button("Confirm", this)
        , cancel_button("Cancel", this)
        , tiles_left_text("Tiles left:", this)
        , tiles_left_count("", this)
    {
        setFixedSize(size);
        setMouseTracking(true);

        tiles_left_text.setAlignment(Qt::AlignCenter);
        tiles_left_count.setAlignment(Qt::AlignCenter);
        tiles_left_text.setMouseTracking(true);
        tiles_left_count.setMouseTracking(true);
        
        std::vector<QPushButton*> buttons
        {
            &play_button,
            &pass_button,
            &clear_button,
            &shuffle_button,
            &swap_button,
            &confirm_button,
            &cancel_button
        };

        for (auto button : buttons)
        {
            button->setFixedWidth(button_width);
            button->adjustSize();
            button->setMouseTracking(true);
        }

        play_pass_stack.setFixedWidth(button_width);
        clear_shuffle_confirm_stack.setFixedWidth(button_width);
        swap_cancel_stack.setFixedWidth(button_width);

        play_pass_stack.setMouseTracking(true);
        clear_shuffle_confirm_stack.setMouseTracking(true);
        swap_cancel_stack.setMouseTracking(true);
        
        play_pass_stack.addWidget(&play_button);
        play_pass_stack.addWidget(&pass_button);
        clear_shuffle_confirm_stack.addWidget(&clear_button);
        clear_shuffle_confirm_stack.addWidget(&shuffle_button);
        clear_shuffle_confirm_stack.addWidget(&confirm_button);
        swap_cancel_stack.addWidget(&swap_button);
        swap_cancel_stack.addWidget(&cancel_button);

        button_layout.addWidget(&play_pass_stack, 0, 0);
        button_layout.addWidget(&clear_shuffle_confirm_stack, 0, 1);
        button_layout.addWidget(&swap_cancel_stack, 0, 2);
        button_layout.addWidget(&tiles_left_text, 0, 3);
        button_layout.addWidget(&tiles_left_count, 0, 4);

        setLayout(&button_layout);
    }
    
    ButtonPanel::~ButtonPanel()
    { }
    
    QPushButton* ButtonPanel::getPlayButton()
    {
        return &play_button;
    }
    
    QPushButton* ButtonPanel::getPassButton()
    {
        return &pass_button;
    }
    
    QPushButton* ButtonPanel::getClearButton()
    {
        return &clear_button;
    }
    
    QPushButton* ButtonPanel::getShuffleButton()
    {
        return &shuffle_button;
    }
    
    QPushButton* ButtonPanel::getSwapButton()
    {
        return &swap_button;
    }
    
    QPushButton* ButtonPanel::getConfirmButton()
    {
        return &confirm_button;
    }
    
    QPushButton* ButtonPanel::getCancelButton()
    {
        return &cancel_button;
    }
    
    void ButtonPanel::showPlayClearSwapButtons()
    {
        play_pass_stack.setCurrentWidget(&play_button);
        clear_shuffle_confirm_stack.setCurrentWidget(&clear_button);
        swap_cancel_stack.setCurrentWidget(&swap_button);

        play_pass_stack.setDisabled(false);
        return;
    }
    
    void ButtonPanel::showPassShuffleSwapButtons()
    {
        play_pass_stack.setCurrentWidget(&pass_button);
        clear_shuffle_confirm_stack.setCurrentWidget(&shuffle_button);
        swap_cancel_stack.setCurrentWidget(&swap_button);

        play_pass_stack.setDisabled(false);

        return;
    }
    
    void ButtonPanel::showConfirmCancelButtons()
    {
        play_pass_stack.setCurrentWidget(&pass_button);
        clear_shuffle_confirm_stack.setCurrentWidget(&confirm_button);
        swap_cancel_stack.setCurrentWidget(&cancel_button);

        play_pass_stack.setDisabled(true);

        return;
    }
    
    void ButtonPanel::setTileCount(int a_count)
    {
        tiles_left = a_count;
        drawTileCounter();
        return;
    }
    
    void ButtonPanel::drawTileCounter()
    {
        QFont count_font{"Monospace", size.height() / 2};
        tiles_left_count.setFont(count_font);
        tiles_left_count.setText(QString::number(tiles_left));
        return;
    }
}
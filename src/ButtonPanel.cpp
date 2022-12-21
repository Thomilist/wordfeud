#include "ButtonPanel.hpp"

namespace wf
{
    ButtonPanel::ButtonPanel(QSize a_size, QWidget* a_parent)
        : QWidget(a_parent)
        , size(a_size)
        , button_width(a_size.width() / 4)
        , play_pass_buttons(this)
        , clear_shuffle_buttons(this)
        , play_button("Play", this)
        , pass_button("Pass", this)
        , clear_button("Clear", this)
        , shuffle_button("Shuffle", this)
        , swap_button("Swap", this)
        , tiles_left_text("Tiles left:", this)
        , tiles_left_count("", this)
    {
        setFixedSize(size);

        tiles_left_text.setAlignment(Qt::AlignRight);
        tiles_left_count.setAlignment(Qt::AlignCenter);
        
        std::vector<QPushButton*> buttons
        {
            &play_button,
            &pass_button,
            &clear_button,
            &shuffle_button,
            &swap_button
        };

        for (auto button : buttons)
        {
            button->setFixedWidth(button_width);
            button->adjustSize();
        }

        play_pass_buttons.setFixedWidth(button_width);
        clear_shuffle_buttons.setFixedWidth(button_width);
        
        play_pass_buttons.addWidget(&play_button);
        play_pass_buttons.addWidget(&pass_button);
        clear_shuffle_buttons.addWidget(&clear_button);
        clear_shuffle_buttons.addWidget(&shuffle_button);
        button_layout.addWidget(&play_pass_buttons, 0, 0);
        button_layout.addWidget(&clear_shuffle_buttons, 0, 1);
        button_layout.addWidget(&swap_button, 0, 2);
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
    
    void ButtonPanel::showPlayClearButtons()
    {
        play_pass_buttons.setCurrentWidget(&play_button);
        clear_shuffle_buttons.setCurrentWidget(&clear_button);
        return;
    }
    
    void ButtonPanel::showPassShuffleButtons()
    {
        play_pass_buttons.setCurrentWidget(&pass_button);
        clear_shuffle_buttons.setCurrentWidget(&shuffle_button);
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
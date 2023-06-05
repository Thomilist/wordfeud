#include "PlayerAI.hpp"


namespace wf
{
    PlayerAI::PlayerAI(
        QString a_display_name,
        QColor a_color,
        Settings* a_settings,
        RenderedTile* a_selection,
        VirtualBoard* a_board)
        : Player(
            a_display_name,
            a_color,
            a_settings,
            a_selection)
        , board(
            a_settings,
            a_board)
    {
        if (a_settings->getBoardDimensions().width() > a_settings->getBoardDimensions().height())
        {
            longest_possible_word = a_settings->getBoardDimensions().width();
        }
        else
        {
            longest_possible_word = a_settings->getBoardDimensions().height();
        }

        for (auto word : a_settings->getLanguage()->getWordList())
        {
            if (word.size() <= longest_possible_word)
            {
                word_list.insert(word);
            }
        }
        
        check_turn_timer.setTimerType(Qt::VeryCoarseTimer);
        connect(&check_turn_timer, &QTimer::timeout, this, &PlayerAI::playIfTurn);
        check_turn_timer.start(1000);
    }
    
    PlayerAI::~PlayerAI()
    { }
    
    void PlayerAI::playIfTurn()
    {
        if (hasTurn())
        {
            check_turn_timer.stop();
            executePlay();
        }

        return;
    }
    
    void PlayerAI::executePlay()
    {
        check_turn_timer.start(1000);
        return;
    }
}
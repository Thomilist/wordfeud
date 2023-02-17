#include "PlayerAI.hpp"


namespace wf
{
    PlayerAI::PlayerAI(
        QString a_display_name,
        QColor a_color,
        Settings* a_settings,
        Tile* a_selection,
        Board* a_board)
        : Player(
            a_display_name,
            a_color,
            a_settings,
            a_selection)
        , board(a_board)
    {
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
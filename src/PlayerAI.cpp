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
    
    void PlayerAI::makeReducedWordList()
    {
        reduced_word_list.clear();
        virtual_board.setWithBoard(board);

        std::unordered_set<QChar, QCharHash> letters_in_hand;
        std::unordered_set<QChar, QCharHash> letters_on_board;

        for (int collumn = 0; collumn < getHand()->getGridDimensions().width(); ++collumn)
        {
            for (int row = 0; row < getHand()->getGridDimensions().height(); ++row)
            {
                if (getHand()->getTileAtPosition(collumn, row)->getLetter() != nullptr)
                {
                    letters_in_hand.insert(getHand()->getTileAtPosition(collumn, row)->getLetter()->getText());
                }
            }
        }

        for (int collumn = 0; collumn < settings->getBoardDimensions().width(); ++collumn)
        {
            for (int row = 0; row < settings->getBoardDimensions().height(); ++row)
            {
                if (virtual_board.getVirtualTileAtPosition(collumn, row)->getLetter() != nullptr)
                {
                    letters_on_board.insert(virtual_board.getVirtualTileAtPosition(collumn, row)->getLetter()->getText());
                }
            }
        }

        int letters_from_hand;
        int letters_from_board;

        for (const auto& word : word_list)
        {
            letters_from_hand = 0;
            letters_from_board = 0;
            
            for (const auto& character : word)
            {
                if (letters_in_hand.contains(character))
                {
                    ++letters_from_hand;
                }

                if (letters_on_board.contains(character))
                {
                    ++letters_from_board;
                }
            }

            // At least one letters from the hand must be used
            if (letters_from_hand == 0)
            {
                continue;
            }

            // At least one letter from the board must be used, unless it is the first turn,
            // in which case the board is empty
            if (letters_on_board.size() > 0 && letters_from_board == 0)
            {
                continue;
            }

            reduced_word_list.insert(word);
        }

        return;
    }
}
#include "Player.hpp"

namespace wf
{
    Player::Player(
        QString a_display_name,
        const QSize& a_hand_size,
        const QSize& a_tile_size,
        Tile* a_selection,
        QWidget* a_parent)
        : display_name(a_display_name)
        , hand_size(a_hand_size)
    {
        hand = new Board{BoardType::Hand, a_hand_size, a_tile_size, a_selection, a_parent};
    }
    
    Player::~Player()
    { }
    
    Board* Player::getHand()
    {
        return hand;
    }
    
    void Player::fillHand(LetterPool* a_letter_pool)
    {
        for (int row = 0; row < hand_size.height(); ++row)
        {
            for (int collumn = 0; collumn < hand_size.width(); ++collumn)
            {
                Tile* tile = hand->getTileAtPosition(collumn, row);

                if (tile == nullptr)
                {
                    return;
                }

                if (tile->getLetter() != nullptr)
                {
                    continue;
                }

                Letter* new_letter = a_letter_pool->getRandomLetter();

                if (new_letter == nullptr)
                {
                    return;
                }

                tile->placeLetter(new_letter);
            }
        }

        return;
    }
}
#include "Player.hpp"

namespace wf
{
    Player::Player(const QSize& a_hand_size, const QSize& a_tile_size, Tile* a_selection, QWidget* a_parent)
        : hand(a_hand_size, a_tile_size, a_selection, a_parent)
        , hand_size(a_hand_size)
    { }
    
    Player::~Player()
    { }
    
    Board* Player::getHand()
    {
        return &hand;
    }
    
    void Player::fillHand(LetterPool* a_letter_pool)
    {
        for (int row = 0; row < hand_size.height(); ++row)
        {
            for (int collumn = 0; collumn < hand_size.width(); ++collumn)
            {
                Tile* tile = hand.getTileAtPosition(collumn, row);

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
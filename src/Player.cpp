#include "Player.hpp"

namespace wf
{
    Player::Player(
        QString a_display_name,
        QColor a_color,
        Settings* a_settings,
        Tile* a_selection,
        QWidget* a_parent)
        : display_name(a_display_name)
        , name_color(a_color)
        , settings(a_settings)
    {
        hand = new Board{BoardType::Hand, a_settings, a_selection, a_parent};
        hand_size = settings->getHandDimensions();
    }
    
    Player::~Player()
    {
        delete hand;
    }
    
    Board* Player::getHand() const
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
                    continue;
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
    
    int Player::availableSpacesInHand()
    {
        int spaces = 0;
        
        for (int collumn = 0; collumn < hand_size.width(); ++collumn)
        {
            for (int row = 0; row < hand_size.height(); ++row)
            {
                Tile* tile = hand->getTileAtPosition(collumn, row);

                if (tile == nullptr)
                {
                    continue;
                }

                if (tile->getLetter() == nullptr)
                {
                    ++spaces;
                }
            }
        }

        return spaces;
    }
    
    void Player::addLetterToHand(Letter* a_letter)
    {
        for (int collumn = 0; collumn < hand_size.width(); ++collumn)
        {
            for (int row = 0; row < hand_size.height(); ++row)
            {
                Tile* tile = hand->getTileAtPosition(collumn, row);

                if (tile == nullptr)
                {
                    continue;
                }

                if (tile->getLetter() == nullptr)
                {
                    tile->placeLetter(a_letter);
                    return;
                }
            }
        }

        return;
    }
    
    void Player::addPoints(int a_points)
    {
        score += a_points;
        return;
    }
    
    int Player::getScore()
    {
        return score;
    }
    
    QString Player::getScoreAsText() const
    {
        return QString::number(score);
    }
    
    QString Player::getDisplayName() const
    {
        return display_name;
    }
    
    void Player::setTurn(bool a_has_turn)
    {
        has_turn = a_has_turn;
        return;
    }
    
    bool Player::hasTurn()
    {
        return has_turn;
    }
    
    void Player::setColor(QColor a_color)
    {
        name_color = a_color;
        return;
    }
    
    QColor Player::getColor()
    {
        return name_color;
    }
    
    int Player::getLetterPenaltyPoints()
    {
        // Calculate penalty points based on remaining tiles in hand
        
        if (getHand()->getLetterCount() == 0)
        {
            return 0;
        }
        
        int penalty = 0;
        Tile* tile;
        
        for (int collumn = 0; collumn < getHand()->getGridDimensions().width(); ++collumn)
        {
            for (int row = 0; row < getHand()->getGridDimensions().height(); ++row)
            {
                tile = getHand()->getTileAtPosition(collumn, row);
                
                if (tile->getLetter() != nullptr)
                {
                    penalty += tile->getLetter()->getPoints();
                }
            }
        }

        return penalty;
    }
}
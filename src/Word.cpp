#include "Word.hpp"

namespace wf
{
    Word::Word()
    { }
    
    Word::~Word()
    { }
    
    void Word::appendLetter(VirtualTile* a_tile)
    {
        tiles.push_back(a_tile);
        return;
    }
    
    QString Word::getWordAsText() const
    {
        QString text;
        
        for (const auto tile : tiles)
        {
            text.append(tile->getLetter()->getText());
        }

        return text;
    }
    
    int Word::calculatePoints() const
    {
        int points = 0;

        // Raw letter value and letter bonuses
        for (const auto tile : tiles)
        {
            Letter* letter = tile->getLetter();
            Modifier* modifier = tile->getModifier();
            int letter_value = letter->getPoints();

            if (modifier != nullptr && !modifier->isUsed() && (modifier->getType() == ModifierType::Letter))
            {
                letter_value *= modifier->getMultiplier();
            }

            points += letter_value;
        }

        // Word bonuses
        for (const auto tile : tiles)
        {
            Modifier* modifier = tile->getModifier();

            if (modifier != nullptr && !modifier->isUsed() && (modifier->getType() == ModifierType::Word))
            {
                points *= modifier->getMultiplier();
            }
        }

        return points;
    }
    
    bool Word::contains(VirtualTile* a_tile) const
    {
        return std::find(tiles.begin(), tiles.end(), a_tile) != tiles.end();
    }
    
    bool Word::containsAnyOf(std::vector<VirtualTile*> a_tiles) const
    {
        for (const auto tile : a_tiles)
        {
            if (contains(tile))
            {
                return true;
            }
        }

        return false;
    }
    
    int Word::containsHowManyOf(std::vector<VirtualTile*> a_tiles) const
    {
        int count = 0;
        
        for (const auto tile : a_tiles)
        {
            if (contains(tile))
            {
                ++count;
            }
        }

        return count;
    }
    
    int Word::getLength() const
    {
        return tiles.size();
    }
    
    void Word::clear()
    {
        tiles.clear();
        return;
    }

    std::vector<VirtualTile*> Word::getTiles() const
    {
        return tiles;
    }
}
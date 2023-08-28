#include "LetterPool.hpp"
namespace wf
{
    LetterPool::LetterPool()
    { }
    
    LetterPool::~LetterPool()
    { }
    
    void LetterPool::set(std::vector<Letter*> a_letters)
    {
        pool = a_letters;
        non_wildcard_letters.clear();
        wildcard_substitutes.clear();

        for (const auto letter : pool)
        {
            if (!letter->getText().isEmpty() && !non_wildcard_letters.contains(letter->getText()))
            {
                non_wildcard_letters.insert(letter->getText());
            }
        }

        // Second loop needed because substitutes are based on
        // the complete set of non-wildcard letters found in the first loop.
        for (auto letter : pool)
        {
            if (letter->getType() == LetterType::Wildcard)
            {
                wildcard_substitutes[letter] = createWildcardSubstitutes();
            }
        }

        return;
    }
    
    [[nodiscard]] Letter* LetterPool::getRandomLetter()
    {
        if (getRemainingCount() == 0)
        {
            return nullptr;
        }
        
        std::random_device random_seed;
        std::mt19937 rng(random_seed());
        std::uniform_int_distribution<> distribution(0, getRemainingCount() - 1);

        int random_index = distribution(rng);
        Letter* random_letter = pool.at(random_index);
        pool.erase(pool.begin() + random_index);

        emit remainingCountChanged(getRemainingCount());

        return random_letter;
    }
    
    int LetterPool::getRemainingCount() const
    {
        return pool.size();
    }
    
    int LetterPool::getRemainingWildcardCount() const
    {
        int count = 0;
        
        for (auto letter : pool)
        {
            if (letter != nullptr && letter->getType() == LetterType::Wildcard)
            {
                ++count;
            }
        }

        return count;
    }
    
    const std::set<QString>& LetterPool::getNonWildcardLetters() const
    {
        return non_wildcard_letters;
    }
    
    QString LetterPool::getNonWildcardLetterBefore(const QString& a_letter) const
    {
        auto position = non_wildcard_letters.find(a_letter);

        // Not found, return empty string
        if (position == non_wildcard_letters.end())
        {
            return "";
        }

        // Found at first position, return same
        if (position == non_wildcard_letters.begin())
        {
            return a_letter;
        }

        // Found elsewhere, return previous
        return *std::prev(position);
    }
    
    QString LetterPool::getNonWildcardLetterAfter(const QString& a_letter) const
    {
        // If empty, return first
        if (a_letter.isEmpty())
        {
            return *non_wildcard_letters.begin();
        }
        
        auto position = non_wildcard_letters.find(a_letter);

        // Not found, return empty string
        if (position == non_wildcard_letters.end())
        {
            return "";
        }

        // Found at last position, return same
        if (std::next(position) == non_wildcard_letters.end())
        {
            return a_letter;
        }

        // Found elsewhere, return next
        return *std::next(position);
    }
    
    void LetterPool::insertLetter(Letter* a_letter)
    {
        pool.push_back(a_letter);
        emit remainingCountChanged(getRemainingCount());
        return;
    }
    
    std::vector<Letter> LetterPool::createWildcardSubstitutes()
    {
        std::vector<Letter> substitutes;

        for (auto character : non_wildcard_letters)
        {
            substitutes.push_back(Letter(character.at(0), 0));
        }

        return substitutes;
    }

    std::vector<Letter*> LetterPool::getWildcardSubstitutes(Letter* a_letter)
    {
        std::vector<Letter*> substitutes;
        
        for (auto& letter : wildcard_substitutes.at(a_letter))
        {
            substitutes.push_back(&letter);
        }

        return substitutes;
    }
}
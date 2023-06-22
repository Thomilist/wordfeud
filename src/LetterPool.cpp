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

        for (const auto letter : pool)
        {
            if (letter->getText().isLetter() && !non_wildcard_letters.contains(letter->getText()))
            {
                non_wildcard_letters.append(letter->getText());
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
    
    QStringList LetterPool::getNonWildcardLetters() const
    {
        return non_wildcard_letters;
    }
    
    void LetterPool::insertLetter(Letter* a_letter)
    {
        pool.push_back(a_letter);
        emit remainingCountChanged(getRemainingCount());
        return;
    }
}
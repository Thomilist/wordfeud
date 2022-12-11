#include "LetterPool.hpp"
namespace wf
{
    LetterPool::LetterPool()
    {
        
    }
    
    LetterPool::~LetterPool()
    { }
    
    void LetterPool::set(Language& a_language)
    {
        pool.clear();
        
        for (const auto& letter_data : a_language.getLetterList())
        {
            for (int n = 0; n < letter_data.count; ++n)
            {
                Letter letter{letter_data.letter, letter_data.points};
                pool.push_back(letter);
            }
        }

        return;
    }
    
    Letter LetterPool::getRandomLetter()
    {
        std::random_device random_seed;
        std::mt19937 rng(random_seed());
        std::uniform_int_distribution<> distribution(0, getRemainingCount() - 1);

        int random_index = distribution(rng);
        Letter random_letter = pool.at(random_index);
        pool.erase(pool.begin() + random_index);

        return random_letter;
    }
    
    int LetterPool::getRemainingCount()
    {
        return pool.size();
    }
}
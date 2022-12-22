#include "LetterPool.hpp"
namespace wf
{
    LetterPool::LetterPool()
    {
        
    }
    
    LetterPool::~LetterPool()
    { }
    
    void LetterPool::set(std::vector<Letter*> a_letter_pointers)
    {
        pool = a_letter_pointers;
        return;
    }
    
    Letter* LetterPool::getRandomLetter()
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
}
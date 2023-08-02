#include "ModifierPattern.hpp"


namespace wf
{
    ModifierPattern::ModifierPattern()
        : rng(std::default_random_engine{})
    { }
    
    ModifierPattern::~ModifierPattern()
    { }
    
    void ModifierPattern::setModifierShuffling(bool a_enabled)
    {
        shuffle_modifiers = a_enabled;
        return;
    }
    
    bool ModifierPattern::getModifierShuffling() const
    {
        return shuffle_modifiers;
    }
    
    void ModifierPattern::setDistribution(ModifierDistribution a_distribution)
    {
        current_distribution = a_distribution;
        return;
    }
    
    void ModifierPattern::setDistribution(QString a_distribution)
    {
        for (auto distribution : all_distributions)
        {
            if (distribution.second == a_distribution)
            {
                current_distribution = distribution.first;
                return;
            }
        }
        
        return;
    }
    
    const std::vector<Modifier*> ModifierPattern::get()
    {
        std::vector<Modifier*> pattern;
        std::vector<Modifier>* base_pattern;

        switch (current_distribution)
        {
            case ModifierDistribution::Wordfeud:
            {
                base_pattern = &wordfeud_default_pattern;
                break;
            }
            case ModifierDistribution::Scrabble:
            {
                base_pattern = &scrabble_default_pattern;
                break;
            }
            default:
            {
                return pattern;
            }
        }
        
        for (auto& modifier : *base_pattern)
        {
            pattern.push_back(&modifier);
        }

        if (shuffle_modifiers)
        {
            std::shuffle(pattern.begin(), pattern.end(), rng);

            int middle_index = pattern.size() / 2;

            if (pattern.at(middle_index)->getType() != ModifierType::Start)
            {
                for (auto& modifier : pattern)
                {
                    if (modifier->getType() == ModifierType::Start)
                    {
                        std::swap(modifier, pattern.at(middle_index));
                        break;
                    }
                }
            }
        }
        
        return pattern;
    }
    
    ModifierDistribution ModifierPattern::getDistribution() const
    {
        return current_distribution;
    }
    
    const QString ModifierPattern::getDistributionAsText() const
    {
        for (auto distribution : all_distributions)
        {
            if (distribution.first == current_distribution)
            {
                return distribution.second;
            }
        }
        
        return "";
    }
    
    const QString ModifierPattern::getDistributionDisplayName() const
    {
        return getDistributionAsText() % (shuffle_modifiers ? " (shuffled)" : "");
    }
    
    const std::vector<std::pair<ModifierDistribution, QString>> ModifierPattern::getAllDistributions() const
    {
        return all_distributions;
    }
    
    void ModifierPattern::reset()
    {
        for (auto& modifier : wordfeud_default_pattern)
        {
            modifier.setUsed(false);
        }

        return;
    }
}
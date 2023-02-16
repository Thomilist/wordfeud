#include "ModifierPattern.hpp"


namespace wf
{
    ModifierPattern::ModifierPattern()
        : rng(std::default_random_engine{})
    { }
    
    ModifierPattern::~ModifierPattern()
    { }
    
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
    
    const std::vector<Modifier*> ModifierPattern::getModifiers()
    {
        std::vector<Modifier*> modifiers;
        
        for (auto& modifier : default_pattern)
        {
            modifiers.push_back(&modifier);
        }

        if (current_distribution == ModifierDistribution::Random)
        {
            std::shuffle(modifiers.begin(), modifiers.end(), rng);

            int middle_index = modifiers.size() / 2;

            if (modifiers.at(middle_index)->getType() != ModifierType::Start)
            {
                for (auto& modifier : modifiers)
                {
                    if (modifier->getType() == ModifierType::Start)
                    {
                        std::swap(modifier, modifiers.at(middle_index));
                        break;
                    }
                }
            }
        }
        
        return modifiers;
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
    
    const std::vector<std::pair<ModifierDistribution, QString>> ModifierPattern::getAllDistributions() const
    {
        return all_distributions;
    }
}